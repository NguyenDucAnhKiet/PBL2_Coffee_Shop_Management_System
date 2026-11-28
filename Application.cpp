#include "Application.h"
#include "storage/DataManager.h"
#include <iostream>
#include <fstream>

using namespace std;

Application::Application(){
    currentUser = nullptr;
}

bool Application::login(string username, string password){
    currentUser = personManager.login(username, password);
    return currentUser != nullptr;
}

bool Application::initialize(){
    DataManager dm;
    bool success = true;
    ifstream testFile("data/persons.json");
    cout << "[DEBUG] open persons.json: " << testFile.is_open() << endl;

    if (!dm.loadPersons(personManager, "data/persons.json")) success = false;
    cout << "[DEBUG] loaded persons count: " << personManager.getAllPersons().size() << endl;
    if (!dm.loadProducts(productManager, "data/products.json")) success = false;
    if (!dm.loadTables(tableManager, "data/tables.json")) success = false;
    if (!dm.loadOrders(orderManager, productManager, "data/orders.json")) success = false;

    return success;
}

bool Application::saveData(){
    DataManager dm;
    bool success = true;

    if (!dm.savePersons(personManager, "data/persons.json")) success = false;
    if (!dm.saveProducts(productManager, "data/products.json")) success = false;
    if (!dm.saveTables(tableManager, "data/tables.json")) success = false;
    if (!dm.saveOrders(orderManager, "data/orders.json")) success = false;

    return success;
}

void Application::logout(){
    currentUser = nullptr;
}

Person* Application::getCurrentUser() const{
    return currentUser;
}

ProductManager& Application::getProductManager(){
    return productManager;
}

PersonManager& Application::getPersonManager(){
    return personManager;
}

TableManager& Application::getTableManager(){
    return tableManager;
}


OrderManager& Application::getOrderManager(){
    return orderManager;
}

void Application::run(){
    if (!initialize())
        return;

    // Login Screen

    // Main Dashboard

    saveData();
}






