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

        ServerMgr(const DataInput& data_input, DataOutput& data_output);
        void processReq(const DataInput& data_input, DataOutput& data_output);
        void purchaseServer(const DataInput& DataInput, DataOutput& data_output);
        bool preAllocVM(const Request& req, VM vm,  DataOutput& data_output);
        void postAllocVM(const Request& req, VM vm, DataOutput& data_output);
        void delAllocVM(const Request& req, VM vm, DataOutput& data_output);
        void migAllocVM();
};