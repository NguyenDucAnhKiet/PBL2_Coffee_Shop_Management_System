#pragma once

#include <vector>
#include "../models/Order.h"

class OrderManager{
    private:
        std::vector<Order*> orders;
        
    public:
        bool createOrder(std::string orderID, std::string tableID, std::string staffID, std::string orderDate, OrderType type);
        Order* findOrderByID(std::string orderID);
        Order* getActiveOrderByTableID(std::string tableID);

        std::vector<Order*> getAllOrders() const;
        std::vector<Order*> getOrdersByDate(std::string date) const;
        std::vector<Order*> getRecentOrders(int limit = 20) const;

        void addOrder(Order* order);
        bool cancelOrder(std::string orderID);
        bool processCheckout(std::string orderID, double amountGiven);

        int getPaidOrderCountByDate(std::string date) const;
        
        double getRevenueByDate(std::string date) const;
        double totalMonthlyRevenue(int month, int year) const;
        double totalAnnualRevenue(int year) const;

        ~OrderManager();
};