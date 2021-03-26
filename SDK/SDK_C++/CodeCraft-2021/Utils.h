#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <iterator>
#include <iostream>
#include <map>

#define TEST
// #define VSDEBUG
// #define LOG

#ifdef TEST
#ifdef VSDEBUG
const std::string train_data = "D:\\Code\\CodeCraft-2021\\training-2.txt";
const std::string test_output = "D:\\Code\\CodeCraft-2021\\test_output.txt";
#else
const std::string train_data = "../../../training-data/training-1.txt";
const std::string test_output = "test_output.txt";
#endif
#endif

enum Operation{
    ADD, DEL
};

std::vector<std::string> split(std::string str_line);

struct VM{
    // 以下为输入时得到的信息
    std::string type;
    int cpu;
    int memory;
    int is_dual;
    // 以下为分配后产生的信息
    int node_type; // 0：双节点 1：a节点 2：b节点

    VM(std::string type, int cpu, int memory, int is_dual); // 构造函数
    void beAlloc(int node_type);
};

struct Server{
    // 以下为输入时得到的信息
    std::string type;
    int cpu_a;
    int cpu_b;
    int memory_a;
    int memory_b;
    int purchase_cost;
    int run_cost;

    // 以下为被预购买产生的信息
    int purchase_index;
    int c_a_remain;
    int c_b_remain;
    int m_a_remain;
    int m_b_remain;
    bool is_on;
    std::unordered_map<int, VM> vm_map;

    Server(std::string type, int cpu_a, int memory_a, int purchase_cost, int run_cost); // 构造函数
    void bePurchased(int purchase_index);
    bool allocVM(int vm_id, VM& vm);
    void delVM(int vm_id);
};


struct Request{
    Operation op;
    std::string vm_type;
    int vm_id;
#ifdef LOG
    std::string toString();
#endif
};

