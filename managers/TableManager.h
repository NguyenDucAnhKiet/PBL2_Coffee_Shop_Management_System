#pragma once

#include <vector>
#include <string>
#include "../models/Table.h"

class TableManager {
private:
    std::vector<Table*> tables;

public:
    TableManager();
    ~TableManager();

    // Thao tác cơ bản
    bool addTable(const Table& table);
    bool removeTable(const std::string& tableID, bool force = false);
    Table* findTableByID(const std::string& tableID) const;
    std::vector<Table*> getAllTables() const;

    // Cập nhật thông tin bàn
    bool updateTableCapacity(const std::string& tableID, int newCapacity);
    bool updateTableStatus(const std::string& tableID, TableStatus newStatus);

    // Lọc và tìm kiếm
    std::vector<Table*> getAvailableTables() const;
    std::vector<Table*> getOccupiedTables() const;
    std::vector<Table*> getReservedTables() const;
    std::vector<Table*> getTablesByCapacity(int minCapacity) const;
    bool isTableAvailable(const std::string& tableID) const;

    // Nghiệp vụ bàn (Chuyển bàn, Nhận bàn, Trả bàn, Đặt bàn)
    bool switchTable(const std::string& fromTableID, const std::string& toTableID);
    bool occupyTable(const std::string& tableID);
    bool releaseTable(const std::string& tableID);
    bool reserveTable(const std::string& tableID);

    // Thống kê nhanh
    int getTotalTableCount() const;
    int getAvailableTableCount() const;
    int getOccupiedTableCount() const;
};
