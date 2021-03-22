#pragma once

#include "Utils.h"

struct DataInput{
    public:
#ifdef TEST
        FILE* input_f;
#endif
        int server_num;
        int vm_num;
        int day_num;
        int max_cost = 0; // 用于最简易策略，只买最贵不买最对
        std::string  max_type; // 用于最简易策略，只买最贵不买最对
        std::vector<int> reqnum_list; 
        std::unordered_map<std::string, Server> server_map;
        std::unordered_map<std::string, VM> vm_map;
        std::vector<std::vector<Request>> request_list;

        DataInput();
        ~DataInput();
        int readConst();
        void readServer(std::unordered_map<std::string, Server>& server_map);
        void readVM(std::unordered_map<std::string, VM>& vm_map);
        void readRequest(std::vector<int>& reqnum_list, std::vector<std::vector<Request>>& request_list);
};
    