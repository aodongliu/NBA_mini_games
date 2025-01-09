#include "utils/DatabaseManager.hpp"
#include "utils/config.hpp"
#include <stdexcept>
#include <iostream>

DatabaseManager::DatabaseManager() {
    int result = sqlite3_open(DATABASE_PATH.c_str(), &db);
    checkError(result, "Opening database");
}

DatabaseManager::~DatabaseManager() {
    if (db) {
        sqlite3_close(db);
    }
}

std::optional<DatabaseManager::PlayerData> DatabaseManager::getPlayerById(int id) {
    const char* sql = R"(
        SELECT p.id, p.first_name, p.last_name, p.team_id, p.headshot_path,
               s.points_per_game, s.rebounds_per_game, s.assists_per_game
        FROM players p
        LEFT JOIN player_stats s ON p.id = s.player_id
        WHERE p.id = ?
    )";
    
    sqlite3_stmt* stmt;
    int result = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    checkError(result, "Preparing player query");
    
    sqlite3_bind_int(stmt, 1, id);
    
    PlayerData player;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        player.id = sqlite3_column_int(stmt, 0);
        player.firstName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        player.lastName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        player.teamId = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        player.headshotPath = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        player.ppg = sqlite3_column_double(stmt, 5);
        player.rpg = sqlite3_column_double(stmt, 6);
        player.apg = sqlite3_column_double(stmt, 7);
        
        sqlite3_finalize(stmt);
        return player;
    }
    
    sqlite3_finalize(stmt);
    return std::nullopt;
}

std::vector<DatabaseManager::PlayerData> DatabaseManager::getAllPlayers() {
    std::vector<PlayerData> players;
    
    const char* query = R"(
        SELECT p.id, p.first_name, p.last_name, p.team_id, p.headshot_path,
               s.points_per_game, s.rebounds_per_game, s.assists_per_game
        FROM players p
        LEFT JOIN player_stats s ON p.id = s.player_id
    )";

    sqlite3_stmt* stmt;
    int result = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
    checkError(result, "Preparing getAllPlayers query");
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        PlayerData player;
        player.id = sqlite3_column_int(stmt, 0);
        player.firstName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        player.lastName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        player.teamId = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        player.headshotPath = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        player.ppg = sqlite3_column_double(stmt, 5);
        player.rpg = sqlite3_column_double(stmt, 6);
        player.apg = sqlite3_column_double(stmt, 7);
        
        players.push_back(player);
    }
    
    sqlite3_finalize(stmt);
    return players;
}

// ... implement other methods similarly ...

void DatabaseManager::checkError(int result, const std::string& operation) {
    if (result != SQLITE_OK) {
        std::string error = sqlite3_errmsg(db);
        throw std::runtime_error("Database error during " + operation + ": " + error);
    }
} 