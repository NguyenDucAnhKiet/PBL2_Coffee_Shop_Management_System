#pragma once

#include <vector>
#include "../models/Table.h"

class TableManager{
    private:
        std::vector<Table*> tables;
        
    public:
        bool addTable(const Table& table);
        bool removeTable(std::string tableID);
        Table* findTableByID(std::string tableID) const;
        std::vector<Table*> getAllTables() const;
        std::vector<Table*> getAvailableTables() const;
        bool updateTableStatus(std::string tableID, TableStatus newStatus);
        bool isTableAvailable(std::string tableID) const;

        ~TableManager();
};
