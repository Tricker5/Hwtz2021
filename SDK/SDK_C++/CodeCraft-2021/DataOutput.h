#pragma once

#include "Utils.h"

struct DataOutput{
    public:
        int day_cost;
        long total_cost;
        int day_migration;
#ifdef TEST
        std::ofstream output_data;
#endif  
        std::vector<int> purchase_num_list;
        std::vector<std::unordered_map<std::string, int>> purchase_list;
        std::vector<std::vector<std::pair<int, std::string>>> alloc_list;
        
        DataOutput();
        ~DataOutput();
#ifdef TEST
        void printDayOutput();
        void printDayPurchaseNum();
        void printDayPurchaseInfo();
        void printDayMigNum();
        void printDayMigInfo();
        void printDayAllocInfo();
        void printTotalCost();
#endif
};