#pragma once

#include "managers/ProductManager.h"
#include "managers/PersonManager.h"
#include "managers/TableManager.h"
#include "managers/OrderManager.h"

class Application{
    private:
        ProductManager productManager;
        PersonManager personManager;
        TableManager tableManager;
        OrderManager orderManager;

        Person* currentUser;

    public:
        Application();

        bool initialize();
        bool saveData();

        void logout();
        bool login(std::string username, std::string password);

        Person* getCurrentUser() const;

        ProductManager& getProductManager();
        PersonManager& getPersonManager();
        TableManager& getTableManager();
        OrderManager& getOrderManager();

        void run();
};