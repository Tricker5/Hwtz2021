#pragma once

#include "Utils.h"

struct DataOutput{
    public:
        int day_cost;
        int energy_cost;
        int hardware_cost;
        long total_cost;
        int day_migration;
        int energy_cost_temp;
        int hardware_cost_temp;
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
        std::vector<std::vector<std::pair<int, int>>> alloc_list;
        // N-1天的购买列表
        std::vector<std::unordered_map<std::string, int>> purchase_list_temp;
        std::vector<std::vector<std::pair<int, int>>> alloc_list_temp;
        
        DataOutput();
        ~DataOutput();
        void addNewDayPurchase();
        void addNewDayAlloc();
        void backup();
        void recover();
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