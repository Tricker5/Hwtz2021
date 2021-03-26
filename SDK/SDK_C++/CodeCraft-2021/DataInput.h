#pragma once

#include "Utils.h"

struct DataInput{
    public:
#ifdef TEST
#ifdef VSDEBUG
        std::ifstream input_f;
#else
        FILE* input_f;
#endif
#endif
        int server_num;
        int vm_num;
        int day_num;
        // int max_cost = 0; // 用于最简易策略，只买最贵不买最对
        // std::string  max_type; // 用于最简易策略，只买最贵不买最对
        std::vector<int> reqnum_list; 
        std::unordered_map<std::string, Server> server_map; // 通过服务器型号找到服务器
        std::unordered_map<std::string, VM> vm_map; // 通过虚拟机型号找到虚拟机
        std::vector<std::vector<Request>> request_list; 

        std::vector<std::string> server_list; // 用于从排序好的服务器列表中选服务器
        std::map<int, std::string, std::greater<int>> sort_server_map; // 排序服务器成本排序

        DataInput();
        ~DataInput();
        int readConst();
        void readServer();
        void readVM();
        void readRequest();
};
    