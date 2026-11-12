#pragma once

#include <string>
#include "../common/Enums.h"
#include "../json.hpp"

class Table {
private:
    std::string tableID;
    TableStatus status;
    int capacity;

public:
    Table();
    Table(std::string tableID, TableStatus status, int capacity);

    // Getters
    std::string getTableID() const;
    TableStatus getStatus() const;
    int getCapacity() const;

    // Setters
    void setTableID(const std::string& tableID);
    void setStatus(TableStatus status);
    bool setCapacity(int capacity);

    // Business methods
    void occupyTable();
    void releaseTable();
    void reserveTable();
    bool isAvailable() const;
    bool isOccupied() const;
    bool isReserved() const;
    bool canAccommodate(int peopleCount) const;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Table, tableID, status, capacity)
};
