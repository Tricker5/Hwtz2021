#include "DataInput.h"


DataInput::DataInput(){
    std::ifstream input_data(train_data);
    server_num = readConst(input_data);
    readServer(input_data, server_map);
    vm_num = readConst(input_data);
    readVM(input_data, vm_map);
    day_num = readConst(input_data);
    readRequest(input_data, reqnum_list, request_list);
    input_data.close();
}

int DataInput::readConst(std::ifstream& input_data){
    std::string str_const;
    getline(input_data, str_const);
    return stoi(str_const);
}

void DataInput::readServer(std::ifstream& input_data, std::unordered_map<std::string, Server>& server_map){
    std::string str_server;
    for(int count = 0; count < server_num; ++count){
        getline(input_data, str_server);
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
        // 找到最贵的服务器
        if(max_cost < s.purchase_cost){
            max_cost = s.purchase_cost;
            max_type = s.type;
        }
    }
}

void DataInput::readVM(std::ifstream& input_data, std::unordered_map<std::string, VM>& vm_map){
    std::string str_vm;
    for(int count = 0; count < vm_num; ++count){
        getline(input_data, str_vm);
        std::vector<std::string> str_vec = split(str_vm);
        VM vm;
        vm.type = str_vec[0];
        vm.cpu = stoi(str_vec[1]);
        vm.memory = stoi(str_vec[2]);
        vm.is_dual = stoi(str_vec[3]);
        vm_map.emplace(vm.type, vm);
    }
}

void DataInput::readRequest(std::ifstream& input_data, std::vector<int>& reqnum_list, std::vector<std::vector<Request>>& request_list){
    std::string str_req;
    for(int count_day = 0, request_num = 0; count_day < day_num; ++count_day){
        request_num = readConst(input_data);
        reqnum_list.push_back(request_num);
        std::vector<Request> day_req;
        for(int count_request = 0; count_request < request_num; ++count_request){
            getline(input_data, str_req);
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

