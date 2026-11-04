#include "PersonManager.h"

using namespace std;

PersonManager::~PersonManager(){
    for (Person* p : persons){
        delete p;
    }
    persons.clear();
}

bool PersonManager::addPerson(const Person& person){
    //Kiem tra trung lap ID truoc khi them 
    if (findPersonByID(person.getPersonID()) != nullptr)
        return false;
    
    persons.push_back(new Person(person));
    return true;
}

bool PersonManager::disablePerson(string personID){
    Person* p = findPersonByID(personID);
    if (p != nullptr){
        p->getAccount().deactivate();
        return true;
    }
    return false;
}

vector<Person*> PersonManager::findPersonsByName(string nameKeyword) const{
    vector<Person*> result;
    for (Person* p : persons){
        if (p->getFullName().find(nameKeyword) != string::npos){
            result.push_back(p);
        }
    }
    return result;
}

Person* PersonManager::findPersonByID(string personID){
    for (Person* p : persons){
        if (p->getPersonID() == personID){
            return p;
        }
    }
    return nullptr;
}

Person* PersonManager::login(const string& username, const string& password){
    for (Person* person : persons){
        Account acc = person->getAccount();

        if (acc.getUsername() == username 
            && acc.verifyPassword(password)
            && acc.isActive())
        {
            return person;    
        }
    }
    return nullptr;
}

vector<Person*> PersonManager::getAllPersons() const{
    return persons;
}

bool PersonManager::updatePersonInfo(string personID, string newPhone, double newSalary, Role newRole){
    Person* p = findPersonByID(personID);
    if (p != nullptr){
        p->setPhone(newPhone);
        p->setSalary(newSalary);
        p->getAccount().setRole(newRole);
        return true;
    }
    return false;
}