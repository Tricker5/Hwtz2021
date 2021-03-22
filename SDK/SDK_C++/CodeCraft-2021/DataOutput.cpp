#include "DataOutput.h"

DataOutput::DataOutput(){
#ifdef TEST
    output_f = freopen(test_output.c_str(), "w", stdout);
#endif
    day_cost = 0;
    total_cost = 0;
    day_migration = 0;
}

DataOutput::~DataOutput(){
#ifdef TEST
    fclose(output_f);
#endif
}



void DataOutput::printDayOutput(){
    printDayPurchaseNum();
    printDayPurchaseInfo();
    printDayMigNum();
    printDayMigInfo();
    printDayAllocInfo();
}

void DataOutput::printDayPurchaseNum(){
    std::cout << "(purchase, " << purchase_list.back().size() << ")" << std::endl;;
}

void DataOutput::printDayPurchaseInfo(){
    for(auto item : purchase_list.back()){
        std::cout << "(" << item.first << ", " << item.second << ")" << std::endl;
    }
}

void DataOutput::printDayMigNum(){
    std::cout << "(migration, 0)" << std::endl;
}

void DataOutput::printDayMigInfo(){

}

void DataOutput::printDayAllocInfo(){
    for(auto item : alloc_list.back()){
        if(item.second.empty())
            std::cout << "(" << item.first << ")" << std::endl;
        else
            std::cout << "(" << item.first << ", " << item.second << ")" << std::endl;
    }
}

#ifdef TEST
void DataOutput::printTotalCost(){
    std::cout << std::endl << std::endl << "Total cost: " << total_cost << std::endl;
}
#endif