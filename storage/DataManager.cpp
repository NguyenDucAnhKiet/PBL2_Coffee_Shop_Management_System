#include "DataManager.h"
#include <fstream>
#include <sstream>
#include <vector>
#include "../json.hpp"

using namespace std;
using json = nlohmann::json;

bool DataManager::loadProducts(ProductManager& pm, const string& filename){
    ifstream file(filename);
    if (!file.is_open())
        return false;

    try {
        json j;
        file >> j;
        
        for (const auto& item : j) {
            Product p = item.get<Product>();
            pm.addProduct(p);
        }
    } catch (...) {
        file.close();
        return false;
    }
    
    file.close();
    return true;
}

bool DataManager::saveProducts(const ProductManager& pm, const string& filename){
    ofstream file(filename);
    if (!file.is_open())
        return false;

    vector<Product*> products = pm.getAllProducts();
    json j = json::array();
    
    for (Product* p : products){
        j.push_back(*p);
    }
    
    file << j.dump(4);
    file.close();
    return true;
}

bool DataManager::loadTables(TableManager& tm, const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) return false;

    try {
        json j;
        file >> j;
        
        for (const auto& item : j) {
            Table t = item.get<Table>();
            tm.addTable(t);
        }
    } catch (...) {
        file.close();
        return false;
    }
    
    file.close();
    return true;
}

bool DataManager::saveTables(const TableManager& tm, const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) return false;

    vector<Table*> tables = tm.getAllTables();
    json j = json::array();
    
    for (Table* t : tables) {
        j.push_back(*t);
    }
    
    file << j.dump(4);
    file.close();
    return true;
}

bool DataManager::loadPersons(PersonManager& pm, const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) return false;

    try {
        json j;
        file >> j;
        
        for (const auto& item : j) {
            Person p = item.get<Person>();
            pm.addPerson(p);
        }
    } catch (...) {
        file.close();
        return false;
    }
    
    file.close();
    return true;
}

bool DataManager::savePersons(const PersonManager& pm, const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) return false;

    vector<Person*> persons = pm.getAllPersons();
    json j = json::array();
    
    for (Person* p : persons) {
        j.push_back(*p);
    }
    
    file << j.dump(4);
    file.close();
    return true;
}

bool DataManager::loadOrders(OrderManager& om, ProductManager& pm, const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) return false;

    try {
        json j;
        file >> j;
        
        for (const auto& item : j) {
            Order* o = new Order(item.get<Order>());
            om.addOrder(o);
        }
    } catch (...) {
        file.close();
        return false;
    }
    
    file.close();
    return true;
}

bool DataManager::saveOrders(const OrderManager& om, const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) return false;

    vector<Order*> orders = om.getAllOrders();
    json j = json::array();
    
    for (Order* o : orders) {
        j.push_back(*o);
    }
    
    file << j.dump(4);
    file.close();
    return true;
}