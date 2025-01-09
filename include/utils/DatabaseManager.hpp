#pragma once
#include <sqlite3.h>
#include <string>
#include <vector>
#include <memory>
#include <optional>

class Player;
class Team;

class DatabaseManager {
public:
    struct PlayerData {
        int id;
        std::string firstName;
        std::string lastName;
        std::string teamId;
        std::string headshotPath;
        float ppg;
        float rpg;
        float apg;
    };

    struct TeamData {
        std::string id;
        std::string name;
        std::string abbreviation;
        std::string logoPath;
    };

    DatabaseManager();
    ~DatabaseManager();

    std::optional<PlayerData> getPlayerById(int id);
    std::optional<TeamData> getTeamById(const std::string& id);
    std::vector<PlayerData> getAllPlayers();
    std::vector<TeamData> getAllTeams();
    std::vector<PlayerData> getPlayersByTeam(const std::string& teamId);

private:
    sqlite3* db;
    void checkError(int result, const std::string& operation);
}; 