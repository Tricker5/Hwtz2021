#include "DataInput.h"


DataInput::DataInput(){
#ifdef TEST
#ifdef VSDEBUG
    input_f.open(train_data);
#else
    input_f = std::freopen(train_data.c_str(), "r", stdin);
#endif
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
#ifdef VSDEBUG
    input_f.close();
#else
    fclose(input_f);
#endif
#endif
}

int DataInput::readConst(){
    std::string str_const;
#ifdef VSDEBUG
    getline(input_f, str_const);
#else
    getline(std::cin, str_const);
#endif
    return stoi(str_const);
}

void DataInput::readServer(){
    std::string str_server;
    for(int count = 0; count < server_num; ++count){
#ifdef  VSDEBUG
        getline(input_f, str_server);
#else
        getline(std::cin, str_server);
#endif
        std::vector<std::string> str_vec = split(str_server); 
        Server s(str_vec[0], stoi(str_vec[1]) / 2, stoi(str_vec[2]) / 2, stoi(str_vec[3]), stoi(str_vec[4]));
        server_map.emplace(s.type, s);
        // 利用map对服务器成本进行排序
        sort_server_map.emplace(s.purchase_cost, s.type);
    }
    for(const auto& item : sort_server_map){
        server_list.push_back(item.second);
    }
}

void DataInput::readVM(){
    std::string str_vm;
    for(int count = 0; count < vm_num; ++count){
#ifdef VSDEBUG
        getline(input_f, str_vm);
#else
        getline(std::cin, str_vm);
#endif
        std::vector<std::string> str_vec = split(str_vm);
        VM vm(str_vec[0], stoi(str_vec[1]), stoi(str_vec[2]), stoi(str_vec[3]));
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
#ifdef VSDEBUG
            getline(input_f, str_req);
#else
            getline(std::cin, str_req);
#endif
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

