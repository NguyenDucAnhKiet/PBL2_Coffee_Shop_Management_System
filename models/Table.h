#pragma once

#include <string>
#include "../json.hpp"
#include "../common/Enums.h"

class Table{
    private:
        std::string tableID;
        TableStatus status;
        int capacity;

    public:
        Table();
        
        Table(
            std::string tableID,
            TableStatus status,
            int capacity
        );

        std::string getTableID() const;
        TableStatus getStatus() const;
        int getCapacity() const;

        void occupyTable();
        void releaseTable();

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Table, tableID, status, capacity)
};