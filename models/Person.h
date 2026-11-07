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
        std::string gender;
        std::string shift;
        std::string dob;

    public:
        Person();

        Person(
            std::string personID,
            std::string fullName,
            std::string phone,
            double salary,
            std::string hireDate,
            Account account,
            std::string gender,
            std::string shift,
            std::string dob
        );

        std::string getPersonID() const;
        std::string getFullName() const;
        std::string getPhone() const;
        std::string getGender() const;
        std::string getShift() const;
        std::string getDob() const;
        double getSalary() const;
        std::string getHireDate() const;
        Account& getAccount();

        bool setPhone(const std::string& phone);
        bool setSalary(double salary);
        bool setShift(const std::string& newShift);
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Person, personID, fullName, phone, salary, hireDate, account, gender, shift, dob)
};