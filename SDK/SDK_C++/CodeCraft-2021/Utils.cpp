#include "Utils.h"

std::vector<std::string> split(std::string str_line){
    std::vector<std::string> str_vec;
    std::string str_temp;
    for(auto c : str_line){
        if(c != ',' && c != ' ' && c != '(' && c != ')'){
            str_temp += c;
        }else{
            if(!str_temp.empty()){
                str_vec.push_back(str_temp);
                str_temp = "";
            }
        }
    }
    return str_vec;
}

Server::Server(std::string type, int cpu_a, int memory_a, int purchase_cost, int run_cost){
    this->type = type;
    this->cpu_a = cpu_a;
    this->cpu_b = cpu_a;
    this->memory_a = memory_a;
    this->memory_b = memory_a;
    this->purchase_cost = purchase_cost;
    this->run_cost = run_cost;
    c_a_remain = cpu_a;
    c_b_remain = cpu_a;
    m_a_remain = memory_a;
    m_b_remain = memory_a;
    is_on = false;
}

void Server::bePurchased(int purchase_index){
    this->purchase_index = purchase_index;
}

bool Server::allocVM(int vm_id, VM& vm){
    if(vm.is_dual){
        if(c_a_remain > vm.cpu / 2 &&
            c_b_remain > vm.cpu / 2 &&
            m_a_remain > vm.memory / 2 &&
            m_b_remain > vm.memory / 2){
                vm.beAlloc(0);
                c_a_remain -= vm.cpu / 2;
                c_b_remain -= vm.cpu / 2;
                m_a_remain -= vm.memory / 2;
                m_b_remain -= vm.memory / 2;
                vm_map.emplace(vm_id, vm);
                return true;
            }
    }else{
        if(c_a_remain > vm.cpu &&
            m_a_remain > vm.memory){
                vm.beAlloc(1);
                c_a_remain -= vm.cpu;
                m_a_remain -= vm.memory;
                vm_map.emplace(vm_id, vm);
                return true;
        }else if(c_b_remain > vm.cpu &&
                    m_b_remain > vm.memory){
                vm.beAlloc(2);
                c_b_remain -= vm.cpu;
                m_b_remain -= vm.memory;
                vm_map.emplace(vm_id, vm);
                return true;    
        }
    }
    return false;
}

void Server::delVM(int vm_id){
    int vm_cpu = vm_map.at(vm_id).cpu;
    int vm_memory = vm_map.at(vm_id).memory;
    int node_type = vm_map.at(vm_id).node_type;
    switch(node_type){
        case 0:
            c_a_remain += vm_cpu / 2;
            c_b_remain += vm_cpu / 2;
            m_a_remain += vm_memory / 2;
            m_b_remain += vm_memory / 2;
            break;
        case 1:
            c_a_remain += vm_cpu;
            m_a_remain += vm_memory;
            break;
        case 2:
            c_b_remain += vm_cpu;
            m_b_remain += vm_memory;
            break;
        default:
            break;
    }
    vm_map.erase(vm_id);
}

VM::VM(std::string type, int cpu, int memory, int is_dual){
    this->type = type;
    this->cpu = cpu;
    this->memory = memory;
    this->is_dual = is_dual;
}

void VM::beAlloc(int node_type){
    this->node_type = node_type;
}