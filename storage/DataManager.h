#pragma once

#include <string>
#include "../managers/ProductManager.h"
#include "../managers/TableManager.h"
#include "../managers/PersonManager.h"
#include "../managers/OrderManager.h"

class DataManager{
    public:
        static bool loadProducts(ProductManager& pm, const std::string& filename = "data/products.json");
        static bool saveProducts(const ProductManager& pm, const std::string& filename = "data/products.json");

        static bool loadTables(TableManager& tm, const std::string& filename = "data/tables.json");
        static bool saveTables(const TableManager& tm, const std::string& filename = "data/tables.json");

        static bool loadPersons(PersonManager& pm, const std::string& filename = "data/persons.json");
        static bool savePersons(const PersonManager& pm, const std::string& filename = "data/persons.json");

        static bool loadOrders(OrderManager& om, ProductManager& pm, const std::string& filename = "data/orders.json");
        static bool saveOrders(const OrderManager& om, const std::string& filename = "data/orders.json");
};