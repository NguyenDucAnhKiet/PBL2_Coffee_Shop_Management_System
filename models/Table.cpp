#include "Table.h"

using namespace std;

Table::Table(){
}

Table::Table(
    string tableID,
    TableStatus status,
    int capacity
){
    this->tableID = tableID;
    this->status = status;
    this->capacity = capacity;
}

string Table::getTableID() const {
    return tableID;
}

TableStatus Table::getStatus() const {
    return status;
}

int Table::getCapacity() const {
    return capacity;
}

void Table::occupyTable(){
    status = TableStatus::OCCUPIED;
}

void Table::releaseTable(){
    status = TableStatus::EMPTY;
}