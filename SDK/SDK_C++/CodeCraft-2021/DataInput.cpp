#include "DataInput.h"


DataInput::DataInput(){
#ifdef TEST
    input_f = std::freopen(train_data.c_str(), "r", stdin);
#endif
    server_num = readConst();
    readServer();
    vm_num = readConst();
    readVM();
    day_num = readConst();
    readRequest();
}

DataInput::~DataInput(){
#ifdef TEST
    fclose(input_f);
#endif
}

int DataInput::readConst(){
    std::string str_const;
    getline(std::cin, str_const);
    return stoi(str_const);
}

void DataInput::readServer(){
    std::string str_server;
    for(int count = 0; count < server_num; ++count){
        getline(std::cin, str_server);
        std::vector<std::string> str_vec = split(str_server); 
        Server s;
        s.type = str_vec[0];
        s.cpu_a = stoi(str_vec[1]) / 2;
        s.cpu_b = s.cpu_a;
        s.memory_a = stoi(str_vec[2]) / 2;
        s.memory_b = s.memory_a;
        s.purchase_cost = stoi(str_vec[3]);
        s.run_cost = stoi(str_vec[4]);
        server_map.emplace(s.type, s);
        sort_server_map.emplace(s.purchase_cost, s.type);
        // server_list.push_back(s.type);
        // // 找到最贵的服务器
        // if(max_cost < s.purchase_cost){
        //     max_cost = s.purchase_cost;
        //     max_type = s.type;
        // }
    }
    for(const auto& item : sort_server_map){
        server_list.push_back(item.second);
        // std::cout << item.first << " " << item.second << std::endl;
    }
}

void DataInput::readVM(){
    std::string str_vm;
    for(int count = 0; count < vm_num; ++count){
        getline(std::cin, str_vm);
        std::vector<std::string> str_vec = split(str_vm);
        VM vm;
        vm.type = str_vec[0];
        vm.cpu = stoi(str_vec[1]);
        vm.memory = stoi(str_vec[2]);
        vm.is_dual = stoi(str_vec[3]);
        vm_map.emplace(vm.type, vm);
    }
}

void DataInput::readRequest(){
    std::string str_req;
    for(int count_day = 0, request_num = 0; count_day < day_num; ++count_day){
        request_num = readConst();
        reqnum_list.push_back(request_num);
        std::vector<Request> day_req;
        for(int count_request = 0; count_request < request_num; ++count_request){
            getline(std::cin, str_req);
            std::vector<std::string> str_vec = split(str_req);
            Request req;
            if("add" == str_vec[0]) {
                req.op = ADD;
                req.vm_type = str_vec[1];
                req.vm_id = stoi(str_vec[2]);
            }
            else{
                req.op = DEL;
                req.vm_id = stoi(str_vec[1]);
            }
            day_req.push_back(req);
        }
        request_list.push_back(day_req);
    }
}

