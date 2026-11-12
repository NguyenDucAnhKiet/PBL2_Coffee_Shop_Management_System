#pragma once

#include <string>
#include "../common/Enums.h"
#include <vector>
#include "OrderItem.h"
#include "../json.hpp"

class Order{
    private:
        std::string orderID;
        std::string employeeID;
        std::string tableID;
        std::string orderDate;
        std::vector<OrderItem> items;
        OrderType type;
        OrderStatus status;
        double amountGiven;
        double changeReturn;

    public:
        Order();

        Order(
            std::string orderID,
            std::string employeeID,
            std::string tableID,
            std::string orderDate,
            OrderType type
        );

        Order(
            std::string orderID,
            std::string employeeID,
            std::string tableID,
            std::string orderDate,
            std::vector<OrderItem> items,
            OrderType type,
            OrderStatus status,
            double amountGiven,
            double changeReturn
        );

        std::string getOrderID() const;
        std::string getEmployeeID() const;
        std::string getTableID() const;
        void setTableID(const std::string& newTableID);
        std::string getOrderDate() const;
        std::vector<OrderItem> getItems() const;
        OrderType getType() const;
        OrderStatus getStatus() const;
        double getAmountGiven() const;
        double getChangeReturn() const;

        bool addItem(const OrderItem& item);
        bool removeItem(std::string productID);
        bool updateItemQuantity(std::string productID, int newQuantity);
        double calculateTotal() const;
        bool processPayment(double amountGiven);

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Order, orderID, employeeID, tableID, orderDate, items, type, status, amountGiven, changeReturn)
};