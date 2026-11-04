#include "Person.h"

using namespace std;

Person::Person(){
}

Person::Person(
    string personID,
    string fullName,
    string phone,
    double salary,
    string hireDate,
    Account account
){
    this->personID = personID;
    this->fullName = fullName;
    this->phone = phone;
    this->salary = salary;
    this->hireDate = hireDate;
    this->account = account;
}

string Person::getPersonID() const {
    return personID;
}

string Person::getFullName() const {
    return fullName;
}

string Person::getPhone() const {
    return phone;
}

double Person::getSalary() const {
    return salary;
}

string Person::getHireDate() const {
    return hireDate;
}

Account& Person::getAccount(){
    return account;
}

bool Person::setPhone(const string& phone){
    if (phone.empty())
        return false;

    if (phone.length() != 10)
        return false;
    
    for (char c : phone){
        if (!isdigit(c))
            return false;
    }

    this->phone = phone;
    return true;
}

bool Person::setSalary(double salary){
    if (salary < 0)
        return false;

    this->salary = salary;
    return true;    
}
