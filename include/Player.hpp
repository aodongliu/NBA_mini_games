#pragma once

#include <string>

class Player{
public:
    Player(const std::string& firstName, const std::string& lastName);

    const std::string& getFirstName( ) const { return firstName; };
    const std::string& getLastName() const { return lastName; };


private:
    std::string firstName;
    std::string lastName;

};