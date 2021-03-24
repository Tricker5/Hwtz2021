#pragma once

#include "Utils.h"

struct DataOutput{
    public:
        int day_cost;
        long total_cost;
        int day_migration;
        int day_cost_temp;
        int total_cost_temp;
#ifdef TEST
#ifdef VSDEBUG
        std::ofstream output_f;
#else
        FILE* output_f;
#endif
#endif  
        // N天的购买列表
        std::vector<std::unordered_map<std::string, int>> purchase_list;
        std::vector<std::vector<std::pair<int, std::string>>> alloc_list;
        // N-1天的购买列表
        std::vector<std::unordered_map<std::string, int>> purchase_list_temp;
        std::vector<std::vector<std::pair<int, std::string>>> alloc_list_temp;
        
        DataOutput();
        ~DataOutput();
        void printDayOutput();
        void printDayPurchaseNum();
        void printDayPurchaseInfo();
        void printDayMigNum();
        void printDayMigInfo();
        void printDayAllocInfo();
#ifdef TEST
        void printTotalCost();
#endif
};