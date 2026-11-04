#pragma once

#include <string>
#include "../common/Enums.h"
#include "../json.hpp"

class Account{
    private:
        std::string username;
        std::string password;
        bool active;
        Role role;

    public:
        Account();

        Account(
            std::string username,
            std::string password,
            bool active,
            Role role
        );

        std::string getUsername() const;
        std::string getPassword() const;
        bool isActive() const;
        Role getRole() const;

        void setRole(Role role);
        bool changePassword(const std::string& oldPassword, const std::string& newPassword);

        void activate();
        void deactivate();

        bool verifyPassword(const std::string& inputPassword) const;
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Account, username, password, active, role)
};