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
    Account account,
    string gender,
    string shift,
    string dob
){
    this->personID = personID;
    this->fullName = fullName;
    this->phone = phone;
    this->salary = salary;
    this->hireDate = hireDate;
    this->account = account;
    this->gender = gender;
    this->shift = shift;
    this->dob = dob;
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

string Person::getGender() const {
    return gender;
}

string Person::getShift() const {
    return shift;
}

string Person::getDob() const {
    return dob;
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

bool Person::setShift(const string& newShift){
    if (newShift.empty())
        return false;
        
    this->shift = newShift;
    return true;
}