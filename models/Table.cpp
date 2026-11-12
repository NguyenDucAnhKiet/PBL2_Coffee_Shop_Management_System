#include "Table.h"

using namespace std;

Table::Table()
    : tableID(""),
      status(TableStatus::EMPTY),
      capacity(0)
{
}

Table::Table(string tableID, TableStatus status, int capacity)
    : tableID(tableID),
      status(status),
      capacity(capacity)
{
}

// ======================
// Getters
// ======================

string Table::getTableID() const {
    return tableID;
}

TableStatus Table::getStatus() const {
    return status;
}

int Table::getCapacity() const {
    return capacity;
}

// ======================
// Setters
// ======================

void Table::setTableID(const string& tableID) {
    this->tableID = tableID;
}

void Table::setStatus(TableStatus status) {
    this->status = status;
}

bool Table::setCapacity(int capacity) {
    if (capacity > 0) {
        this->capacity = capacity;
        return true;
    }
    return false;
}

// ======================
// Business methods
// ======================

void Table::occupyTable() {
    status = TableStatus::OCCUPIED;
}

void Table::releaseTable() {
    status = TableStatus::EMPTY;
}

void Table::reserveTable() {
    status = TableStatus::RESERVED;
}

bool Table::isAvailable() const {
    return status == TableStatus::EMPTY;
}

bool Table::isOccupied() const {
    return status == TableStatus::OCCUPIED;
}

bool Table::isReserved() const {
    return status == TableStatus::RESERVED;
}

bool Table::canAccommodate(int peopleCount) const {
    return peopleCount > 0 && peopleCount <= capacity;
}
