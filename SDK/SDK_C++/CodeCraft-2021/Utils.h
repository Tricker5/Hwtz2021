#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <iterator>
#include <iostream>
#include <map>

// #define TEST
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

