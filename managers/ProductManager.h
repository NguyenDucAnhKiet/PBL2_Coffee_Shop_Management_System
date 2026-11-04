#pragma once

#include <vector>
#include "../models/Product.h"

class ProductManager{
    private:
        std::vector<Product*> products;
        
    public:
        bool addProduct(const Product& product);
        bool removeProduct(std::string productID);
        bool updateProductPrice(std::string productID, double newPrice);

        Product* findProductByID(std::string productID);
        std::vector<Product*> searchProducts(const std::string& keyword);
        std::string toLowerCase(std::string str);

        std::vector<Product*> getAllProducts() const;
        std::vector<Product*> getProductsByCategory(std::string category) const;

        ~ProductManager();
};