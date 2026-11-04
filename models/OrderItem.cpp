#include "OrderItem.h"

OrderItem::OrderItem(){
}

OrderItem::OrderItem(
    Product product,
    int quantity
){
    this->product = product;
    this->quantity = quantity;
}

Product OrderItem::getProduct() const {
    return product;
}

int OrderItem::getQuantity() const {
    return quantity;
}

bool OrderItem::setQuantity(int quantity){
    if (quantity <= 0)
        return false;
    
    this->quantity = quantity;
    return true;
}

double OrderItem::getSubTotal() const {
    return product.getPrice() * quantity;
}