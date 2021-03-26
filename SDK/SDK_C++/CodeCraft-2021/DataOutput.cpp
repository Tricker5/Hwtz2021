#include "DataOutput.h"

DataOutput::DataOutput(){
#ifdef TEST
#ifdef VSDEBUG
    output_f.open(test_output);
#else
    output_f = freopen(test_output.c_str(), "w", stdout);
#endif
#endif
    day_cost = 0;
    total_cost = 0;
    day_migration = 0;
}

DataOutput::~DataOutput(){
#ifdef TEST
#ifdef VSDEBUG
    output_f.close();
#else
    fclose(output_f);
#endif
#endif
}

void DataOutput::addNewDayPurchase(){
    std::unordered_map<std::string, int> day_purchase;
    purchase_list.push_back(day_purchase);
}

void DataOutput::addNewDayAlloc(){
    std::vector<std::pair<int, int>> day_alloc;
    alloc_list.push_back(day_alloc);
}

void DataOutput::backup(){
    purchase_list_temp = purchase_list;
    alloc_list_temp = alloc_list;
    day_cost_temp = day_cost;
    total_cost_temp = total_cost;
    energy_cost_temp = energy_cost;
    hardware_cost_temp = hardware_cost;
}

void DataOutput::recover(){
    purchase_list = purchase_list_temp;
    alloc_list = alloc_list_temp;
    day_cost = day_cost_temp;
    total_cost = total_cost_temp;
    energy_cost = energy_cost_temp;
    hardware_cost = hardware_cost_temp;
}

void DataOutput::printDayOutput(){
    printDayPurchaseNum();
    printDayPurchaseInfo();
    printDayMigNum();
    printDayMigInfo();
    printDayAllocInfo();
}

void DataOutput::printDayPurchaseNum(){
#ifdef VSDEBUG
    output_f
#else
    std::cout 
#endif
        << "(purchase, " << purchase_list.back().size() << ")" << std::endl;
}

void DataOutput::printDayPurchaseInfo(){
    for(auto item : purchase_list.back()){
#ifdef VSDEBUG
    output_f
#else
    std::cout 
#endif 
        << "(" << item.first << ", " << item.second << ")" << std::endl;
    }
}

void DataOutput::printDayMigNum(){
#ifdef VSDEBUG
    output_f
#else
    std::cout 
#endif
        << "(migration, 0)" << std::endl;
}

void DataOutput::printDayMigInfo(){

}

void DataOutput::printDayAllocInfo(){
    std::string node_type_list[] = {"", ", A", ", B"};
    for(auto item : alloc_list.back()){
#ifdef VSDEBUG
            output_f
#else
            std::cout 
#endif
                 << "(" << item.first << node_type_list[item.second] << ")" << std::endl;
    }
}

#ifdef TEST
void DataOutput::printTotalCost(){
    total_cost = energy_cost + hardware_cost;
#ifdef VSDEBUG
    output_f
#else
    std::cout 
#endif
        << std::endl << std::endl << "Total cost: " << total_cost << std::endl << 
            "Hardware cost: " << hardware_cost << std::endl <<
            "Energy cost: " << energy_cost << std::endl;
}
#endif