#include "TableManager.h"

using namespace std;

TableManager::~TableManager(){
    for (Table* t : tables){
        delete t;
    }
    tables.clear();
}

bool TableManager::addTable(const Table& table){
    if (findTableByID(table.getTableID()) != nullptr)
        return false;

    tables.push_back(new Table(table));
    return true;   
}

bool TableManager::removeTable(string tableID){
    for (auto it = tables.begin(); it != tables.end(); ++it){
        if ((*it)->getTableID() == tableID){
            delete *it;
            tables.erase(it);
            return true;
        }
    }
    return false;
}

Table* TableManager::findTableByID(string tableID) const{
    for (Table* t : tables){
        if (t->getTableID() == tableID){
            return t;
        }
    }
    return nullptr;
}

vector<Table*> TableManager::getAllTables() const{
    return tables;
}

vector<Table*> TableManager::getAvailableTables() const{
    vector<Table*> result;
    for (Table* t : tables){
        if (t->getStatus() == TableStatus::EMPTY){
            result.push_back(t);
        }
    }
    return result;
}

bool TableManager::updateTableStatus(string tableID, TableStatus newStatus){
    Table* t = findTableByID(tableID);
    if (t != nullptr){
        if (newStatus == TableStatus::EMPTY){
            t->releaseTable();
        }
        else if (newStatus == TableStatus::OCCUPIED){
            t->occupyTable();
        }
        return true;
    }
    return false;
}

bool TableManager::isTableAvailable(string tableID) const{
    Table* t = findTableByID(tableID);
    if (t != nullptr){
        return t->getStatus() == TableStatus::EMPTY;
    }
    return false;
}