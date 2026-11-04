#pragma once

#include <string>
#include "Account.h"
#include "../json.hpp"

class Person{
    private:
        std::string personID;
        std::string fullName;
        std::string phone;
        double salary;
        std::string hireDate;
        Account account;

    public:
        Person();

        Person(
            std::string personID,
            std::string fullName,
            std::string phone,
            double salary,
            std::string hireDate,
            Account account
        );

        std::string getPersonID() const;
        std::string getFullName() const;
        std::string getPhone() const;
        double getSalary() const;
        std::string getHireDate() const;
        Account& getAccount();

        bool setPhone(const std::string& phone);
        bool setSalary(double salary);
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Person, personID, fullName, phone, salary, hireDate, account)
};