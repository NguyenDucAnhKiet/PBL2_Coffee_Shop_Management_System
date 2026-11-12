#include "TableManager.h"

using namespace std;

TableManager::TableManager() {
}

TableManager::~TableManager() {
    for (Table* t : tables) {
        delete t;
    }
    tables.clear();
}

bool TableManager::addTable(const Table& table) {
    if (table.getTableID().empty() || table.getCapacity() <= 0)
        return false;

    if (findTableByID(table.getTableID()) != nullptr)
        return false;

    tables.push_back(new Table(table));
    return true;
}

bool TableManager::removeTable(const string& tableID, bool force) {
    for (auto it = tables.begin(); it != tables.end(); ++it) {
        if ((*it)->getTableID() == tableID) {
            // Không cho xóa nếu bàn đang có khách hoặc đang đặt trước (trừ khi ép buộc force = true)
            if (!force && (*it)->getStatus() != TableStatus::EMPTY) {
                return false;
            }
            delete *it;
            tables.erase(it);
            return true;
        }
    }
    return false;
}

Table* TableManager::findTableByID(const string& tableID) const {
    for (Table* t : tables) {
        if (t->getTableID() == tableID) {
            return t;
        }
    }
    return nullptr;
}

vector<Table*> TableManager::getAllTables() const {
    return tables;
}

bool TableManager::updateTableCapacity(const string& tableID, int newCapacity) {
    Table* t = findTableByID(tableID);
    if (t != nullptr) {
        return t->setCapacity(newCapacity);
    }
    return false;
}

bool TableManager::updateTableStatus(const string& tableID, TableStatus newStatus) {
    Table* t = findTableByID(tableID);
    if (t != nullptr) {
        t->setStatus(newStatus);
        return true;
    }
    return false;
}

vector<Table*> TableManager::getAvailableTables() const {
    vector<Table*> result;
    for (Table* t : tables) {
        if (t->getStatus() == TableStatus::EMPTY) {
            result.push_back(t);
        }
    }
    return result;
}

vector<Table*> TableManager::getOccupiedTables() const {
    vector<Table*> result;
    for (Table* t : tables) {
        if (t->getStatus() == TableStatus::OCCUPIED) {
            result.push_back(t);
        }
    }
    return result;
}

vector<Table*> TableManager::getReservedTables() const {
    vector<Table*> result;
    for (Table* t : tables) {
        if (t->getStatus() == TableStatus::RESERVED) {
            result.push_back(t);
        }
    }
    return result;
}

vector<Table*> TableManager::getTablesByCapacity(int minCapacity) const {
    vector<Table*> result;
    for (Table* t : tables) {
        if (t->getCapacity() >= minCapacity) {
            result.push_back(t);
        }
    }
    return result;
}

bool TableManager::isTableAvailable(const string& tableID) const {
    Table* t = findTableByID(tableID);
    if (t != nullptr) {
        return t->isAvailable();
    }
    return false;
}

bool TableManager::switchTable(const string& fromTableID, const string& toTableID) {
    if (fromTableID == toTableID) return false;

    Table* fromTable = findTableByID(fromTableID);
    Table* toTable = findTableByID(toTableID);

    if (!fromTable || !toTable) return false;
    // Bàn nguồn phải đang có khách hoặc đặt trước
    if (fromTable->getStatus() == TableStatus::EMPTY) return false;
    // Bàn đích phải đang trống
    if (toTable->getStatus() != TableStatus::EMPTY) return false;

    // Chuyển trạng thái sang bàn đích và giải phóng bàn nguồn
    toTable->setStatus(fromTable->getStatus());
    fromTable->releaseTable();
    return true;
}

bool TableManager::occupyTable(const string& tableID) {
    Table* t = findTableByID(tableID);
    if (t != nullptr) {
        t->occupyTable();
        return true;
    }
    return false;
}

bool TableManager::releaseTable(const string& tableID) {
    Table* t = findTableByID(tableID);
    if (t != nullptr) {
        t->releaseTable();
        return true;
    }
    return false;
}

bool TableManager::reserveTable(const string& tableID) {
    Table* t = findTableByID(tableID);
    if (t != nullptr) {
        t->reserveTable();
        return true;
    }
    return false;
}

int TableManager::getTotalTableCount() const {
    return static_cast<int>(tables.size());
}

int TableManager::getAvailableTableCount() const {
    return static_cast<int>(getAvailableTables().size());
}

int TableManager::getOccupiedTableCount() const {
    return static_cast<int>(getOccupiedTables().size());
}
