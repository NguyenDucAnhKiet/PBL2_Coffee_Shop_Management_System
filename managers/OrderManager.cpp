#include "OrderManager.h"
#include <string>

using namespace std;

OrderManager::~OrderManager(){
    for (Order* o : orders){
        delete o;
    }
    orders.clear();
}

bool OrderManager::createOrder(string orderID, string tableID, string staffID, string orderDate, OrderType type){
    if (findOrderByID(orderID) != nullptr)
        return false;

    Order* newOrder = new Order(orderID, staffID, tableID, orderDate, type);
    orders.push_back(newOrder);
    return true;
}

Order* OrderManager::findOrderByID(string orderID){
    for (Order* order : orders){
        if (order->getOrderID() == orderID){
            return order;
        } 
    }
    return nullptr;
}

Order* OrderManager::getActiveOrderByTableID(string tableID){
    for (Order* order : orders){
        if (order->getTableID() == tableID && order->getStatus() == OrderStatus::PENDING){
            return order;
        }
    }
    return nullptr;
}

vector<Order*> OrderManager::getAllOrders() const{
    return orders;
} 

vector<Order*> OrderManager::getOrdersByDate(string date) const{
    vector<Order*> result;
    for (Order* order : orders){
        if (order->getOrderDate() == date){
            result.push_back(order);
        }
    }
    return result;
}

void OrderManager::addOrder(Order* order){
    if (order != nullptr)
        orders.push_back(order);
}

bool OrderManager::cancelOrder(string orderID){
    for (auto it = orders.begin(); it != orders.end(); ++it){
        if ((*it)->getOrderID() == orderID){
            if ((*it)->getStatus() == OrderStatus::PENDING){
                delete *it;
                orders.erase(it);
                return true;
            }
            return false;
        }
    }
    return false;
}

bool OrderManager::processCheckout(string orderID, double amountGiven){
    Order* order = findOrderByID(orderID);
    if (order != nullptr){
        return order->processPayment(amountGiven);
    }
    return false;
}

double OrderManager::totalMonthlyRevenue(int month, int year) const{
    double total = 0.0;
    for (Order* order : orders){
        if (order->getStatus() == OrderStatus::PAID){
            string date = order->getOrderDate(); //DD/MM/YYYY
            if (date.length() >= 10){
                try {
                    int m = stoi(date.substr(3, 2));
                    int y = stoi(date.substr(6, 4));
                    if (m == month && y == year){
                        total += order->calculateTotal();
                    }
                } catch (...){

                }
            }
        }
    }
    return total;
}

double OrderManager::totalAnnualRevenue(int year) const {
    double total = 0.0;
    for (Order* order : orders) {
        if (order->getStatus() == OrderStatus::PAID) {
            string date = order->getOrderDate(); //DD/MM/YYYY
            if (date.length() >= 10) {
                try {
                    int y = stoi(date.substr(6, 4));
                    if (y == year) {
                        total += order->calculateTotal();
                    }
                } catch (...) {

                }
            }
        }
    }
    return total;
}

int OrderManager::getPaidOrderCountByDate(string date) const{
    int count = 0;
    for (Order* order : orders){
        string orderDateOnly = order->getOrderDate().substr(0, 10);
        if (orderDateOnly == date && order->getStatus() == OrderStatus::PAID){
            count++;
        }
    }
    return count;
}

double OrderManager::getRevenueByDate(string date) const{
    double total = 0.0;
    for (Order* order : orders){
        string orderDateOnly = order->getOrderDate().substr(0, 10);
        if (orderDateOnly == date && order->getStatus() == OrderStatus::PAID){
            total += order->calculateTotal();
        } 
    }
    return total;
}

vector<Order*> OrderManager::getRecentOrders(int limit) const{
    vector<Order*> result;

    for (auto it = orders.rbegin(); it != orders.rend(); ++it){
        result.push_back(*it);

        if (result.size() == limit)
            break;
    }
    return result;
}