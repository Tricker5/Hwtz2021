#pragma once

#include "Utils.h"
#include "DataInput.h"
#include "DataOutput.h"

struct ServerMgr{
    public:
        // N天的分配情况;
        std::vector<Server> serv_purchase_list;
        std::unordered_map<int, int> vm_alloc_map;
        // 备份N-1天的分配情况
        std::vector<Server> serv_purchase_list_temp;
        std::unordered_map<int, int> vm_alloc_map_temp;
        // 指向输入输出
        DataInput* data_input;
        DataOutput* data_output;

        ServerMgr(DataInput& data_input, DataOutput& data_output);
        ~ServerMgr();
        void processReq();
        bool purchaseServer(const Request& req, int serv_index);
        bool preAllocVM(const Request& req);
        void postAllocVM(const Request& req);
        void delAllocVM(const Request& req);
        void migAllocVM();
        void backup();
        void recover();
};