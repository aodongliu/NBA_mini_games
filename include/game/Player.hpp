#pragma once
#include <string>
#include "utils/DatabaseManager.hpp"

class Player {
public:
    Player() = default;
    explicit Player(const DatabaseManager::PlayerRecord& record);

    // Basic info
    int getId() const { return id; }
    const std::string& getFirstName() const { return first_name; }
    const std::string& getLastName() const { return last_name; }
    std::string getFullName() const { return first_name + " " + last_name; }
    const std::string& getTeamId() const { return team_id; }
    const std::string& getHeadshotPath() const { return headshot_path; }
    const std::string& getPosition() const { return position; }
    const std::string& getHeight() const { return height; }
    const std::string& getWeight() const { return weight; }
    
    // Updated method to get the team
    DatabaseManager::TeamData getTeam() const {
        return DatabaseManager::getInstance().getTeamById(team_id);
    }

    // Stats
    float getPointsPerGame() const { return points_per_game; }
    float getReboundsPerGame() const { return rebounds_per_game; }
    float getAssistsPerGame() const { return assists_per_game; }
    float getFieldGoalPercentage() const { return field_goal_percentage; }

    // Database operations
    static std::vector<Player> getPlayersAbovePoints(float minPPG);
    static std::vector<Player> getPlayersByTeam(const std::string& teamId);
    static Player getPlayerById(int playerId);

private:
    int id;
    std::string first_name;
    std::string last_name;
    std::string team_id;
    std::string headshot_path;
    std::string position;
    std::string height;
    std::string weight;
    
    float points_per_game = 0.0f;
    float rebounds_per_game = 0.0f;
    float assists_per_game = 0.0f;
    float field_goal_percentage = 0.0f;
    float three_point_percentage = 0.0f;
};