#pragma once
#include <string>
#include "utils/DatabaseManager.hpp"

class Player {
public:
    Player() = default;
    Player(const DatabaseManager::PlayerData& data);
    
    // Getters for data
    const std::string& getFirstName() const { return firstName; }
    const std::string& getLastName() const { return lastName; }
    const std::string& getTeamId() const { return teamId; }
    const std::string& getHeadshotPath() const { return headshotPath; }
    float getPPG() const { return ppg; }
    float getRPG() const { return rpg; }
    float getAPG() const { return apg; }

private:
    std::string firstName;
    std::string lastName;
    std::string teamId;
    std::string headshotPath;
    float ppg = 0.0f;
    float rpg = 0.0f;
    float apg = 0.0f;
};