#include "Account.h"

using namespace std;

Account::Account(){
}

Account::Account(
    string username,
    string password,
    bool active,
    Role role
){
    this->username = username;
    this->password = password;
    this->active = active;
    this->role = role;
}

string Account::getUsername() const {
    return username;
}

string Account::getPassword() const {
    return password;
}

bool Account::isActive() const {
    return active;
}

Role Account::getRole() const {
    return role;
}

void Account::setRole(Role role){
    this->role = role;
}

bool Account::changePassword(const string& oldPassword, const string& newPassword){
    if (!active)
        return false;
    
    if (oldPassword != password)
        return false;
    
    if (newPassword.empty())
        return false;
    
    if (newPassword == password)
        return false;
        
    password = newPassword;
    return true;
}

void Account::activate(){
    active = true;
}

void Account::deactivate(){
    active = false;
}

bool Account::verifyPassword(const string& inputPassword) const{
    return inputPassword == password;
}


