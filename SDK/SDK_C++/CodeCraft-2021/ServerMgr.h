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
        // N天的分配情况;
        std::vector<ServerAlloc> serv_alloc_list;
        std::unordered_map<int, VMAlloc> vm_alloc_map;
        // 备份N-1天的分配情况
        std::vector<ServerAlloc> serv_alloc_list_temp;
        std::unordered_map<int, VMAlloc> vm_alloc_map_temp;
        DataInput* data_input;
        DataOutput* data_output;

        ServerMgr(DataInput& data_input, DataOutput& data_output);
        ~ServerMgr();
        void processReq();
        bool purchaseServer(const Request& req, int serv_index, bool is_min_cost);
        bool preAllocVM(const Request& req, bool is_min_cost);
        void postAllocVM(const Request& req, bool is_min_cost);
        void delAllocVM(const Request& req, bool is_min_cost);
        void migAllocVM();
};