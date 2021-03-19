#include "DataOutput.h"

DataOutput::DataOutput(){
    day_cost = 0;
    total_cost = 0;
#ifdef TEST
    output_data.open(test_output);
#endif
}

DataOutput::~DataOutput(){
#ifdef TEST
    output_data.close();
#endif 
}



#ifdef TEST
void DataOutput::printDayOutput(){
    printDayPurchaseNum();
    printDayPurchaseInfo();
    printDayMigNum();
    printDayMigInfo();
    printDayAllocInfo();
}

void DataOutput::printDayPurchaseNum(){
    output_data << "(purchase, " << purchase_num_list.back() << ")" << std::endl;;
}

void DataOutput::printDayPurchaseInfo(){
    for(auto item : purchase_list.back()){
        output_data << "(" << item.first << ", " << item.second << ")" << std::endl;
    }
}

void DataOutput::printDayMigNum(){
    output_data << "(migration, 0)" << std::endl;
}

void DataOutput::printDayMigInfo(){

}

void DataOutput::printDayAllocInfo(){
    for(auto item : alloc_list.back()){
        if(item.second.empty())
            output_data << "(" << item.first << ")" << std::endl;
        else
            output_data << "(" << item.first << ", " << item.second << ")" << std::endl;
    }
}

void DataOutput::printTotalCost(){
    output_data << std::endl << std::endl << "Total cost: " << total_cost << std::endl;
}
#endif