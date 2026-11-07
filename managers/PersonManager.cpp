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

vector<Person*> PersonManager::searchPersons(string keyword, string shiftFilter, string roleFilter) const{
    vector<Person*> result;
    vector<Person*> allPersons = getAllPersons();

    for (Person* p : allPersons){
        if (!keyword.empty()){
            string nameLower = p->getFullName();
            string idLower = p->getPersonID();
            string kw = keyword;

            transform(nameLower.begin(), nameLower.end(), nameLower.begin(), ::tolower);
            transform(idLower.begin(), idLower.end(), idLower.begin(), ::tolower);
            transform(kw.begin(), kw.end(), kw.begin(), ::tolower);

            if (nameLower.find(kw) == string::npos && idLower.find(kw) == string::npos) continue;
        }

        if (shiftFilter != "All" && p->getShift() != shiftFilter) continue;
        
        if (roleFilter != "All") {
            int roleVal = (int)p->getAccount().getRole();
            if (to_string(roleVal) != roleFilter) continue;
        }
        result.push_back(p);
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

bool PersonManager::updatePersonInfo(string personID, string newPhone, double newSalary, Role newRole, string newShift){
    Person* p = findPersonByID(personID);
    if (p != nullptr){
        p->setPhone(newPhone);
        p->setSalary(newSalary);
        p->getAccount().setRole(newRole);
        p->setShift(newShift);
        return true;
    }
    return false;
}