#include "ServerMgr.h"

ServerAlloc::ServerAlloc(
    std::string server_type, 
    int c_a_remain,
    int c_b_remain,
    int m_a_remain,
    int m_b_remain,
    int run_cost,
    bool is_on):
        payload_count(0){
            this->server_type = server_type;
            this->c_a_remain = c_a_remain;
            this->c_b_remain = c_b_remain;
            this->m_a_remain = m_a_remain;
            this->m_b_remain = m_b_remain;
            this->run_cost = run_cost;
            this->is_on = is_on;
        };

VMAlloc::VMAlloc(int vm_id, std::string vm_type, int alloc_serv, int node_type){
    this->vm_id = vm_id;
    this->vm_type = vm_type;
    this->alloc_serv = alloc_serv;
    this->node_type = node_type;
}


ServerMgr::ServerMgr(DataInput& data_input, DataOutput& data_output){
    this->data_input = &data_input;
    this->data_output = &data_output;
    processReq();
#ifdef TEST
    data_output.printTotalCost();
#endif
}

ServerMgr::~ServerMgr(){
    data_input = nullptr;
    data_output = nullptr;
}

/**
 * @brief 
 * 使用最简单的请求处理策略
 * 对于每个虚拟机请求直接遍历当前服务器，若可以分配则立刻分配之，若都无法分配则买新的服务器
 * @param request_list 
 */
void ServerMgr::processReq(){
    for(const auto day_req : data_input->request_list){
        // 记录当天的购买信息
        std::unordered_map<std::string, int> day_purchase;
        data_output->purchase_list.push_back(day_purchase);
        // 记录当天的分配信息
        std::vector<std::pair<int, std::string>> day_alloc;
        data_output->alloc_list.push_back(day_alloc);
        // 处理当天的请求
        for(const Request req : day_req){
            if(req.op == ADD){
                if(!preAllocVM(req)){
                    purchaseServer(req);
                    postAllocVM(req);
                }
            }else{
                delAllocVM(req);
            }
        }
        // std::cout << data_output->day_cost << std::endl;
        data_output->total_cost += data_output->day_cost; // 累计每天运行成本
        data_output->printDayOutput(); // 输出当天决策信息
    }
}

/**
 * @brief 
 * 对当前请求进行预分配，返回是否预分配成功
 * @param req 
 * @param vm 
 * @param data_output 
 * @return true 
 * @return false 
 */
bool ServerMgr::preAllocVM(const Request& req){
    int count = 0;
    const VM& vm = data_input->vm_map.at(req.vm_type);
    if(vm.is_dual){
        for(auto& serv_alloc : serv_alloc_list){
            if(serv_alloc.c_a_remain > vm.cpu / 2 &&
                serv_alloc.c_b_remain > vm.cpu / 2 &&
                serv_alloc.m_a_remain > vm.memory / 2 &&
                serv_alloc.m_b_remain > vm.memory / 2){
                    VMAlloc vm_alloc(req.vm_id, req.vm_type, count, 0);
                    vm_alloc_map.emplace(req.vm_id, vm_alloc);
                    serv_alloc.c_a_remain -= vm.cpu / 2;
                    serv_alloc.c_b_remain -= vm.cpu / 2;
                    serv_alloc.m_a_remain -= vm.memory / 2;
                    serv_alloc.m_b_remain -= vm.memory / 2;
                    ++serv_alloc.payload_count;
                    if(!serv_alloc.is_on){
                        serv_alloc.is_on = true;
                        data_output->day_cost += serv_alloc.run_cost;
                    }
                    // 记录分配信息
                    data_output->alloc_list.back().push_back(std::make_pair(count, ""));
                    return true;
            }
            ++count;
        }
    }else{
        for(auto& serv_alloc : serv_alloc_list){
            if(serv_alloc.c_a_remain > vm.cpu && serv_alloc.m_a_remain > vm.memory){
                VMAlloc vm_alloc(req.vm_id, req.vm_type, count, 1);
                vm_alloc_map.emplace(req.vm_id, vm_alloc);
                serv_alloc.c_a_remain -= vm.cpu ;
                serv_alloc.m_a_remain -= vm.memory;
                ++serv_alloc.payload_count;
                if(!serv_alloc.is_on){
                    serv_alloc.is_on = true;
                    data_output->day_cost += serv_alloc.run_cost;
                }
                // 记录分配信息
                data_output->alloc_list.back().push_back(std::make_pair(count, "A"));
                return true;
            }else if(serv_alloc.c_b_remain > vm.cpu && serv_alloc.m_b_remain > vm.memory){
                VMAlloc vm_alloc(req.vm_id, req.vm_type, count, 2);
                vm_alloc_map.emplace(req.vm_id, vm_alloc);
                serv_alloc.c_b_remain -= vm.cpu ;
                serv_alloc.m_b_remain -= vm.memory;
                ++serv_alloc.payload_count;
                if(!serv_alloc.is_on){
                    serv_alloc.is_on = true;
                    data_output->day_cost += serv_alloc.run_cost;
                }
                // 记录分配信息
                data_output->alloc_list.back().push_back(std::make_pair(count, "B"));
                return true;
            }
            ++count;
        }
    }
    return false;
}

/**
 * @brief 
 * 最简易策略中，后分配过程是将无法容纳的虚拟机直接置于新购服务器之中
 * @param req 
 * @param vm 
 * @param data_output
 */
void ServerMgr::postAllocVM(const Request& req){
    ServerAlloc& serv_alloc = serv_alloc_list.back();
    const VM& vm = data_input->vm_map.at(req.vm_type);
    int node_type; // 节点信息
    if(vm.is_dual){
        serv_alloc.c_a_remain -= vm.cpu / 2;
        serv_alloc.c_b_remain -= vm.cpu / 2;
        serv_alloc.m_a_remain -= vm.memory / 2;
        serv_alloc.m_b_remain -= vm.memory / 2;
        node_type = 0;
        // 记录分配信息
        data_output->alloc_list.back().push_back(std::make_pair(serv_alloc_list.size() - 1, ""));
    }else{
        serv_alloc.c_a_remain -= vm.cpu ;
        serv_alloc.m_a_remain -= vm.memory;
        node_type = 1;
        // 记录分配信息
        data_output->alloc_list.back().push_back(std::make_pair(serv_alloc_list.size() - 1, "A"));
    }
    ++serv_alloc.payload_count;
    // 新购服务器开机，增加运行成本
    serv_alloc.is_on = true; 
    data_output->day_cost += serv_alloc.run_cost;
    // 记录虚拟机配置信息
    VMAlloc vm_alloc(req.vm_id, req.vm_type, serv_alloc_list.size() - 1, node_type);
    vm_alloc_map.emplace(req.vm_id, vm_alloc);
}

/**
 * @brief 
 * 删除指定虚拟机
 * @param req 
 * @param vm 
 * @param data_output 
 */
void ServerMgr::delAllocVM(const Request& req){
    const VMAlloc& vm_alloc = vm_alloc_map.at(req.vm_id);
    const VM& vm = data_input->vm_map.at(vm_alloc.vm_type);
    ServerAlloc& serv_alloc = serv_alloc_list[vm_alloc.alloc_serv];
    switch(vm_alloc.node_type){
    case 0:
        serv_alloc.c_a_remain += vm.cpu / 2;
        serv_alloc.c_b_remain += vm.cpu / 2;
        serv_alloc.m_a_remain += vm.memory / 2;
        serv_alloc.m_b_remain += vm.memory / 2;
        break;
    case 1:
        serv_alloc.c_a_remain += vm.cpu;
        serv_alloc.m_a_remain += vm.memory;
        break;
    default:
        serv_alloc.c_b_remain += vm.cpu;
        serv_alloc.m_b_remain += vm.memory;
        break;
    }
    // 若无虚拟机则关机，日运行成本降低
    if(--serv_alloc.payload_count == 0){
        serv_alloc.is_on = false;
        data_output->day_cost -= serv_alloc.run_cost;
    }
    vm_alloc_map.erase(req.vm_id);
}

// 暂定
void ServerMgr::migAllocVM(){

}

/**
 * @brief 
 * 薛定谔算法
 * @param data_input 
 * @param data_output 
 */
void ServerMgr::purchaseServer(const Request& req){
    Server serv;
    const VM& vm = data_input->vm_map.at(req.vm_type);
    srand(unsigned(time(0)));
    // int count = 0;
    while(true){
        // ++count;
        serv = data_input->server_map.at(data_input->server_list.at(rand() % data_input->server_num));
        if(vm.is_dual){
            if(serv.cpu_a > vm.cpu / 2 && serv.memory_a > vm.memory / 2)
                break;
        }else{
            if(serv.cpu_a > vm.cpu && serv.memory_a > vm.memory)
                break;
        }
    }
    // std::cout << count << std::endl;
    ServerAlloc serv_alloc(serv.type, serv.cpu_a, serv.cpu_b, serv.memory_a, serv.memory_b, serv.run_cost, false);
    serv_alloc_list.push_back(serv_alloc);
    data_output->total_cost += serv.purchase_cost; // 成本增加
    ++data_output->purchase_list.back()[serv.type]; // 对当天购买的特定服务器进行计数
}


