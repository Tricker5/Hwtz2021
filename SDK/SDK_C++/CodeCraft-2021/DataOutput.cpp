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
    for(auto item : alloc_list.back()){
        if(item.second.empty())
#ifdef VSDEBUG
            output_f
#else
            std::cout 
#endif
                << "(" << item.first << ")" << std::endl;
        else
#ifdef VSDEBUG
            output_f
#else
            std::cout 
#endif
                 << "(" << item.first << ", " << item.second << ")" << std::endl;
    }
}

#ifdef TEST
void DataOutput::printTotalCost(){
#ifdef VSDEBUG
    output_f
#else
    std::cout 
#endif
        << std::endl << std::endl << "Total cost: " << total_cost << std::endl;
}
#endif