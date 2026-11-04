#include "ProductManager.h"

using namespace std;

ProductManager::~ProductManager(){
    for (Product* p : products){
        delete p;
    }
    products.clear();
}

bool ProductManager::checkNameExist(string name){
    for (Product* p : products){
        if (p->getName() == name){
            return true;
        }
    }
    return false;
}

bool ProductManager::addProduct(const Product& product){
    if (findProductByID(product.getProductID()) != nullptr)
        return false;

    if (checkNameExist(product.getName()))
        return false;

    products.push_back(new Product(product));
    return true;
}

bool ProductManager::removeProduct(string productID){
    for (auto it = products.begin(); it != products.end(); ++it){
        if ((*it)->getProductID() == productID){
            delete *it;
            products.erase(it);
            return true;
        }
    }
    return false;   
}

Product* ProductManager::findProductByID(string productID){
    for (Product* p : products){
        if (p->getProductID() == productID){
            return p;
        }
    }
    return nullptr;    
}

vector<Product*> ProductManager::getAllProducts() const{
    return products;
}

vector<Product*> ProductManager::getProductsByCategory(string category) const{
    vector<Product*> result;
    for (Product* p : products){
        if (p->getCategory() == category){
            result.push_back(p);
        }
    }
    return result;
}

bool ProductManager::updateProductPrice(string productID, double newPrice){
    Product* p = findProductByID(productID);
    if (p != nullptr)
        return p->changePrice(newPrice);
    return false;
}

string ProductManager::toLowerCase(string str){
    for (char& c : str){
        c = tolower(c);
    }
    return str;
}

vector<Product*> ProductManager::searchProducts(const string& keyword){
    vector<Product*> results;

    if (keyword.empty())
        return results;

    string lowerKeyword = toLowerCase(keyword);

    for (Product* product : products){
        string lowerName = toLowerCase(product->getName());
        if (lowerName.find(lowerKeyword) != string::npos)
            results.push_back(product);
    }
    return results;
}