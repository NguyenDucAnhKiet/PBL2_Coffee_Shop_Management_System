#include "Product.h"

using namespace std;

Product::Product(){
}

Product::Product(
    string productID,
    string name,
    double price,
    string category,
    bool available
){
    this->productID = productID;
    this->name = name;
    this->price = price;
    this->category = category;
    this->available = available;
}

string Product::getProductID() const {
    return productID;
}

string Product::getName() const {
    return name;
}

double Product::getPrice() const {
    return price;
}

string Product::getCategory() const {
    return category;
}

bool Product::isAvailable() const {
    return available;
}

bool Product::changePrice(double newPrice){
    if (newPrice < 0)
        return false;
    
    price = newPrice;
    return true;
}

void Product::markAvailable(){
    available = true;
}

void Product::markUnavailable(){
    available = false;
}