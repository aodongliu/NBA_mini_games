#pragma once
#include <string>
#include <vector>
#include <memory>
#include <sqlite3.h>

class DatabaseManager {
public:
    // Matches database schema from schema.py exactly
    struct PlayerRecord {
        int id;
        std::string first_name;
        std::string last_name;
        std::string team_id;
        std::string headshot_path;
        std::string position;
        std::string height;
        std::string weight;
        std::string jersey_number;
        std::string season;
        
        int games_played;
        int games_started;
        
        float minutes_per_game;
        float points_per_game;
        float rebounds_per_game;
        float assists_per_game;
        float field_goal_percentage;
        float three_point_percentage;
        float free_throw_percentage;
        float steals_per_game;
        float blocks_per_game;
    };

    struct TeamData {
        std::string id;
        std::string name;
        std::string abbreviation;
        std::string logo_path;
    };

    static DatabaseManager& getInstance();
    std::vector<PlayerRecord> getPlayers(const std::string& whereClause = "", 
                                       const std::vector<std::string>& params = {});
    PlayerRecord getPlayerById(int id);
    std::vector<PlayerRecord> getPlayersByTeam(const std::string& teamId);

private:
    DatabaseManager();
    ~DatabaseManager();
    void initializeConnection();
    void checkError(int result, const std::string& operation);

    sqlite3* db;
};
