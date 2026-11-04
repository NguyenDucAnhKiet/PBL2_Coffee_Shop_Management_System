#pragma once

#include "Product.h"
#include "../json.hpp"

class OrderItem{
    private:
        Product product;
        int quantity;

    public:
        OrderItem();

        OrderItem(
            Product product,
            int quantity
        );

        Product getProduct() const;
        int getQuantity() const;

        bool setQuantity(int quantity);

        double getSubTotal() const;
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(OrderItem, product, quantity)
};