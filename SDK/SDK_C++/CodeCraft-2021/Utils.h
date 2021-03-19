#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <iterator>

#define TEST
// #define LOG

#ifdef TEST
const std::string train_data = "../../../training-data/training-2.txt";
const std::string test_output = "test_output.txt";
#endif

enum Operation{
    ADD, DEL
};

std::vector<std::string> split(std::string str_line);

struct Server{
    std::string type;
    int cpu_a;
    int cpu_b;
    int memory_a;
    int memory_b;
    int purchase_cost;
    int run_cost;
#ifdef LOG
    std::string toString();
#endif
};

struct VM{
    std::string type;
    int cpu;
    int memory;
    int is_dual;
};

struct Request{
    Operation op;
    std::string vm_type;
    int vm_id;
#ifdef LOG
    std::string toString();
#endif
};

