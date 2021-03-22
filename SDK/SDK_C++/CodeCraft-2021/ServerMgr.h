#pragma once

#include "Utils.h"
#include "DataInput.h"
#include "DataOutput.h"

struct ServerAlloc{
    ServerAlloc(std::string server_type, 
        int c_a_remain,
        int c_b_remain,
        int m_a_remain,
        int m_b_remain,
        int run_cost,
        bool is_on);
    int payload_count;
    std::string server_type;
    int c_a_remain;
    int c_b_remain;
    int m_a_remain;
    int m_b_remain;
    int run_cost;
    bool is_on;
};

struct VMAlloc{
    VMAlloc(int vm_id, std::string vm_type, int alloc_serv, int node_type);
    int vm_id;
    std::string vm_type;
    int alloc_serv;
    int node_type; // 0：双节点 1：a节点 2：b节点
};

struct ServerMgr{
    public:
        // int day_cost;
        std::vector<ServerAlloc> serv_alloc_list;
        std::unordered_map<int, VMAlloc> vm_alloc_map;
        DataInput* data_input;
        DataOutput* data_output;

        ServerMgr(DataInput& data_input, DataOutput& data_output);
        ~ServerMgr();
        void processReq();
        void purchaseServer(const Request& req);
        bool preAllocVM(const Request& req);
        void postAllocVM(const Request& req);
        void delAllocVM(const Request& req);
        void migAllocVM();
};