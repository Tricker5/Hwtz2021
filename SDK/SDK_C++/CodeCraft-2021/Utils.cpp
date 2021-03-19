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

#ifdef LOG
std::string Server::toString(){
    return type + "//" +
        std::to_string(cpu_a) + "//" +
        std::to_string(memory_a) + "//"+
        std::to_string(purchase_cost) + "//" +
        std::to_string(run_cost);
}
std::string Request::toString(){
    return (op == ADD ? "add//" : "del//")  +
        vm_type + "//" +
        std::to_string(vm_id);
}
#endif