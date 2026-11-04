#pragma once

#include <vector>
#include "../models/Person.h"

class PersonManager{
    private:
        std::vector<Person*> persons;

    public:
        bool addPerson(const Person& person);
        bool disablePerson(std::string personID);

        std::vector<Person*> getAllPersons() const;
        std::vector<Person*> findPersonsByName(std::string nameKeyword) const;

        Person* findPersonByID(std::string personID);
        Person* login(const std::string& username, const std::string& password);

        bool updatePersonInfo(std::string personID, std::string newPhone, double newSalary, Role newRole);

        ~PersonManager();
};
