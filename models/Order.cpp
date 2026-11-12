#include "Order.h"

using namespace std;

Order::Order(){
}

Order::Order(std::string orderID, std::string employeeID, std::string tableID, std::string orderDate, OrderType type)
    : orderID(orderID), employeeID(employeeID), tableID(tableID), orderDate(orderDate), type(type),
      status(OrderStatus::PENDING), amountGiven(0.0), changeReturn(0.0) {}

Order::Order(std::string orderID, std::string employeeID, std::string tableID, std::string orderDate, 
             std::vector<OrderItem> items, OrderType type, OrderStatus status, 
             double amountGiven, double changeReturn)
    : orderID(orderID), employeeID(employeeID), tableID(tableID), orderDate(orderDate), 
      items(items), type(type), status(status), amountGiven(amountGiven), changeReturn(changeReturn) {}

string Order::getOrderID() const {
    return orderID;
}

string Order::getEmployeeID() const {
    return employeeID;
}

string Order::getTableID() const {
    return tableID;
}

void Order::setTableID(const string& newTableID) {
    this->tableID = newTableID;
}

string Order::getOrderDate() const {
    return orderDate;
}

vector<OrderItem> Order::getItems() const {
    return items;
}

OrderType Order::getType() const {
    return type;
}

OrderStatus Order::getStatus() const {
    return status;
}

double Order::getAmountGiven() const{
    return amountGiven;
}

double Order::getChangeReturn() const{
    return changeReturn;
}

bool Order::addItem(const OrderItem& item){
    if (status == OrderStatus::PAID)
        return false;

    if (!item.getProduct().isAvailable())
        return false;

    if (item.getQuantity() <= 0)
        return false;
        
    for (OrderItem& orderItem : items){
        if (orderItem.getProduct().getProductID()
        == item.getProduct().getProductID()){
            int newQuantity = orderItem.getQuantity() 
            + item.getQuantity();

            if (!orderItem.setQuantity(newQuantity))
                return false;
            return true;
        }
    }

    items.push_back(item);
    return true;
}

bool Order::removeItem(string productID){
    if (status == OrderStatus::PAID)
        return false;

    for (auto it = items.begin(); it != items.end(); ++it){
        if (it->getProduct().getProductID() == productID){
            items.erase(it);
            return true;
        }
    }
    return false;
}

bool Order::updateItemQuantity(string productID, int newQuantity){
    if (status == OrderStatus::PAID)
        return false;

    if (newQuantity <= 0)
        return false;

    for (OrderItem& orderItem : items){
        if (orderItem.getProduct().getProductID() == productID){
            if (orderItem.getQuantity() == newQuantity)
                return false;
            
            if (!orderItem.setQuantity(newQuantity))
                return false;
            return true;
        }
    }

    return false;
}

double Order::calculateTotal() const {
    double total = 0;

    for (const OrderItem& orderItem : items){
        total += orderItem.getSubTotal();
    }
    return total;
}

bool Order::processPayment(double amountGiven){
    if (status == OrderStatus::PAID)
        return false;

    if (items.empty())
        return false;

    double total = calculateTotal();

    if (amountGiven < total)
        return false;

    this->amountGiven = amountGiven;
    changeReturn = amountGiven - total;
    status = OrderStatus::PAID;

    return true;
}

