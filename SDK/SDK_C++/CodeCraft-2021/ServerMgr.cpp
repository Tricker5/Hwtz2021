#include "ServerMgr.h"


ServerMgr::ServerMgr(DataInput& data_input, DataOutput& data_output) {
    this->data_input = &data_input;
    this->data_output = &data_output;
    processReq();
#ifdef TEST
    data_output.printTotalCost();
#endif
}

ServerMgr::~ServerMgr() {
    data_input = nullptr;
    data_output = nullptr;
}

void ServerMgr::backup(){
    serv_purchase_list_temp = serv_purchase_list;
    vm_alloc_map_temp = vm_alloc_map;
}

void ServerMgr::recover(){
    serv_purchase_list = serv_purchase_list_temp;
    vm_alloc_map = vm_alloc_map_temp;
}


/**
 * @brief
 * 使用最简单的请求处理策略
 * 对于每个虚拟机请求直接遍历当前服务器，若可以分配则立刻分配之，若都无法分配则买新的服务器
 * @param request_list
 */
void ServerMgr::processReq() {
    for (const auto day_req : data_input->request_list) {
        migAllocVM();
        // 以下对前一天的状态进行备份
        backup();
        data_output->backup();
        // 当天最小成本服务器
        std::pair<int, int> min_cost_server = {INT32_MAX, INT32_MAX};
        for (int serv_index = 0; serv_index != data_input->server_num / 15 ; ++serv_index) {
            // 预存储当天的购买信息
            data_output->addNewDayPurchase();
            // 预存储当天的分配信息
            data_output->addNewDayAlloc();
            // 判断当前的服务器是否兼容
            bool is_suitable = true;
            // 预处理当天的请求
            for (const Request req : day_req) {
                if (req.op == ADD) {
                    if (!preAllocVM(req)) {
                        if (!purchaseServer(req, serv_index)){
                            is_suitable = false;
                            break;
                        }
                    }
                }
                else {
                    delAllocVM(req);
                }
            }
            if (is_suitable) {
                if(min_cost_server.first > data_output->day_cost){
                    min_cost_server.first = data_output->day_cost;
                    min_cost_server.second = serv_index;
                }
            }
            // 恢复备份
            recover();
            data_output->recover();
        }
        // 存储当天的输出购买信息
        data_output->addNewDayPurchase();
        // 存储当天的输出分配信息
        data_output->addNewDayAlloc();
        for (const Request req : day_req) {
            if (req.op == ADD) {
                if (!preAllocVM(req)) {
                    purchaseServer(req, min_cost_server.second);
                }
            }
            else {
                delAllocVM(req);
            }
        }
        data_output->energy_cost += data_output->day_cost; // 累计每天运行成本
        data_output->printDayOutput(); // 输出当天决策信息
    }
}

/**
 * @brief
 * 对当前请求进行预分配，返回是否预分配成功
 * @param req
 */
bool ServerMgr::preAllocVM(const Request& req) {
    VM vm = data_input->vm_map.at(req.vm_type);
    for(size_t i = 0; i != serv_purchase_list.size(); ++i){
        Server& serv = serv_purchase_list[i];
        if(serv.allocVM(req.vm_id, vm)){
            // 记录分配信息
            vm_alloc_map.emplace(req.vm_id, i);
            if(!serv.is_on){
                serv.is_on = true;
                data_output->day_cost += serv.run_cost;
            }
            // 记录用于输出的分配信息
            data_output->alloc_list.back().push_back(std::make_pair(i, vm.node_type));
            return true;
        }
    }
    return false;
}

/**
 * @brief
 * 删除指定虚拟机
 * @param req
 * @param vm
 * @param data_output
 */
void ServerMgr::delAllocVM(const Request& req) {
    Server& serv = serv_purchase_list.at(vm_alloc_map.at(req.vm_id));
    serv.delVM(req.vm_id);
    // 若无虚拟机则关机，日运行成本降低
    if (serv.vm_map.empty()) {
        serv.is_on = false;
        data_output->day_cost -= serv.run_cost;
    }
    // 删除相关分配信息
    vm_alloc_map.erase(req.vm_id);
}

/**
 * @brief
 * 简单地随便迁移
 */
void ServerMgr::migAllocVM() {
    data_output->addNewDayMigration();
    // 从后往前找卖家
    for (int i = serv_purchase_list.size() - 1; i != -1; --i) {
        Server& serv_out = serv_purchase_list[i];
        // 简单地以负荷为标准
        if (!serv_out.vm_map.empty() && serv_out.vm_map.bucket_count() < 10) {
            // 记录卖家的出货记录
            std::vector<int> id_list;
            for (auto item : serv_out.vm_map) {
                // 从前往后找下家
                for (int j = 0; j != i; ++j) {
                    Server& serv_in = serv_purchase_list[j];
                    // 只迁开机的，只坑老实人
                    if (serv_in.is_on && serv_in.allocVM(item.first, item.second)) {
                        // 更改存储的分配信息
                        vm_alloc_map.at(item.first) = j;
                        // 记录输出的迁移信息
                        std::vector<int> mig_info = { item.first, j, item.second.node_type };
                        data_output->migration_list.back().push_back(mig_info);
                        id_list.push_back(item.first);
                        break;
                    }
                }
            }
            // 卖家清除相关信息
            for (int vm_id : id_list) {
                serv_out.delVM(vm_id);
            }
            // 若无虚拟机则关机，日运行成本降低
            if (serv_out.vm_map.empty()) {
                serv_out.is_on = false;
                data_output->day_cost -= serv_out.run_cost;
            }
        }
    }
}

/**
 * @brief 
 * 购买服务器，没啥策略，买就完事儿了
 * @param req 
 * @param serv_index 
 * @return true 
 * @return false 
 */
bool ServerMgr::purchaseServer(const Request& req, int serv_index) {
    Server serv = data_input->server_map.at(data_input->server_list.at(serv_index));
    VM vm = data_input->vm_map.at(req.vm_type);
    if(serv.allocVM(req.vm_id, vm)){
        // 默认新购服务器直接开机，运行成本增加
        serv.is_on = true;
        data_output->day_cost += serv.run_cost;
        // 记录虚拟机配置信息
        int purchase_index = serv_purchase_list.size();
        vm_alloc_map.emplace(req.vm_id, purchase_index);
        // 记录输出分配信息
        data_output->alloc_list.back().push_back(std::make_pair(purchase_index, vm.node_type));
        serv.bePurchased(purchase_index); // 确认该服务器被购买
        serv_purchase_list.push_back(serv); // 加入已购列表
        data_output->hardware_cost += serv.purchase_cost; // 硬件成本增加
        ++data_output->purchase_list.back()[serv.type]; // 对当天购买的特定服务器进行计数
        return true;
    }else{
        return false;
    }
}


