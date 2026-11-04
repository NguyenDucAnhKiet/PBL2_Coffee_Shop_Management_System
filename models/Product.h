#pragma once

#include <string>
#include "../json.hpp"

class Product{
    private:
        std::string productID;
        std::string name;
        double price;
        std::string category;
        bool available;

    public:
        Product();

        Product(
            std::string productID,
            std::string name,
            double price,
            std::string category,
            bool available
        );

        std::string getProductID() const;
        std::string getName() const;
        double getPrice() const;
        std::string getCategory() const;
        bool isAvailable() const;

        bool changePrice(double newPrice);
        void markAvailable();
        void markUnavailable();

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Product, productID, name, price, category, available)
};