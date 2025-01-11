#include "utils/DatabaseManager.hpp"
#include "utils/config.hpp"
#include <stdexcept>
#include <iostream>

DatabaseManager& DatabaseManager::getInstance() {
    static DatabaseManager instance;
    return instance;
}

DatabaseManager::DatabaseManager() {
    initializeConnection();
}

DatabaseManager::~DatabaseManager() {
    if (db) {
        sqlite3_close(db);
    }
}

void DatabaseManager::initializeConnection() {
    int rc = sqlite3_open(DB_PATH.c_str(), &db);
    if (rc) {
        std::cerr << "Failed to open database at: " << DB_PATH << std::endl;
        std::cerr << "SQLite error: " << sqlite3_errmsg(db) << std::endl;
        throw std::runtime_error("Can't open database: " + std::string(sqlite3_errmsg(db)));
    }
}

void DatabaseManager::checkError(int result, const std::string& operation) {
    if (result != SQLITE_OK) {
        std::string error = "SQLite error during " + operation + ": " + sqlite3_errmsg(db);
        throw std::runtime_error(error);
    }
}

std::vector<DatabaseManager::PlayerRecord> DatabaseManager::getPlayers(
    const std::string& whereClause, 
    const std::vector<std::string>& params) {
    
    std::vector<PlayerRecord> players;
    std::string query = 
        "SELECT p.*, s.* FROM players p "
        "JOIN player_stats s ON p.id = s.player_id ";
    
    if (!whereClause.empty()) {
        query += "WHERE " + whereClause;
    }

    sqlite3_stmt* stmt;
    checkError(sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr), "prepare statement");

    for (size_t i = 0; i < params.size(); i++) {
        checkError(sqlite3_bind_text(stmt, i + 1, params[i].c_str(), -1, SQLITE_STATIC), 
                  "bind parameter");
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        PlayerRecord player;
        player.id = sqlite3_column_int(stmt, 0);
        
        const unsigned char* first_name = sqlite3_column_text(stmt, 1);
        player.first_name = first_name ? reinterpret_cast<const char*>(first_name) : "";
        
        const unsigned char* last_name = sqlite3_column_text(stmt, 2);
        player.last_name = last_name ? reinterpret_cast<const char*>(last_name) : "";
        
        const unsigned char* team_id = sqlite3_column_text(stmt, 3);
        player.team_id = team_id ? reinterpret_cast<const char*>(team_id) : "";
        
        const unsigned char* headshot_path = sqlite3_column_text(stmt, 4);
        player.headshot_path = headshot_path ? reinterpret_cast<const char*>(headshot_path) : "";
        
        const unsigned char* jersey_number = sqlite3_column_text(stmt, 5);
        player.jersey_number = jersey_number ? reinterpret_cast<const char*>(jersey_number) : "";
        
        const unsigned char* position = sqlite3_column_text(stmt, 6);
        player.position = position ? reinterpret_cast<const char*>(position) : "";
        
        const unsigned char* height = sqlite3_column_text(stmt, 7);
        player.height = height ? reinterpret_cast<const char*>(height) : "";
        
        const unsigned char* weight = sqlite3_column_text(stmt, 8);
        player.weight = weight ? reinterpret_cast<const char*>(weight) : "";
        
        const unsigned char* season = sqlite3_column_text(stmt, 10);
        player.season = season ? reinterpret_cast<const char*>(season) : "";
        
        player.games_played = sqlite3_column_int(stmt, 11);
        player.games_started = sqlite3_column_int(stmt, 12);
        
        if (sqlite3_column_type(stmt, 13) != SQLITE_NULL)
            player.minutes_per_game = static_cast<float>(sqlite3_column_double(stmt, 13));
        if (sqlite3_column_type(stmt, 14) != SQLITE_NULL)
            player.field_goal_percentage = static_cast<float>(sqlite3_column_double(stmt, 14));
        if (sqlite3_column_type(stmt, 15) != SQLITE_NULL)
            player.three_point_percentage = static_cast<float>(sqlite3_column_double(stmt, 15));
        if (sqlite3_column_type(stmt, 16) != SQLITE_NULL)
            player.free_throw_percentage = static_cast<float>(sqlite3_column_double(stmt, 16));
        if (sqlite3_column_type(stmt, 17) != SQLITE_NULL)
            player.rebounds_per_game = static_cast<float>(sqlite3_column_double(stmt, 17));
        if (sqlite3_column_type(stmt, 18) != SQLITE_NULL)
            player.assists_per_game = static_cast<float>(sqlite3_column_double(stmt, 18));
        if (sqlite3_column_type(stmt, 19) != SQLITE_NULL)
            player.steals_per_game = static_cast<float>(sqlite3_column_double(stmt, 19));
        if (sqlite3_column_type(stmt, 20) != SQLITE_NULL)
            player.blocks_per_game = static_cast<float>(sqlite3_column_double(stmt, 20));
        if (sqlite3_column_type(stmt, 21) != SQLITE_NULL)
            player.points_per_game = static_cast<float>(sqlite3_column_double(stmt, 21));
        
        players.push_back(player);
    }

    sqlite3_finalize(stmt);
    return players;
}

DatabaseManager::PlayerRecord DatabaseManager::getPlayerById(int id) {
    auto players = getPlayers("p.id = ?", {std::to_string(id)});
    if (players.empty()) {
        throw std::runtime_error("Player not found");
    }
    return players[0];
}

std::vector<DatabaseManager::PlayerRecord> DatabaseManager::getPlayersByTeam(const std::string& teamId) {
    return getPlayers("p.team_id = ?", {teamId});
}

std::vector<DatabaseManager::TeamData> DatabaseManager::getTeams() {
    std::vector<TeamData> teams;
    const char* query = "SELECT id, name, abbreviation, logo_path, conference, division FROM teams";
    
    sqlite3_stmt* stmt;
    checkError(sqlite3_prepare_v2(db, query, -1, &stmt, nullptr), "prepare teams statement");
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        TeamData team;
        
        const unsigned char* id = sqlite3_column_text(stmt, 0);
        team.id = id ? reinterpret_cast<const char*>(id) : "";
        
        const unsigned char* name = sqlite3_column_text(stmt, 1);
        team.name = name ? reinterpret_cast<const char*>(name) : "";
        
        const unsigned char* abbreviation = sqlite3_column_text(stmt, 2);
        team.abbreviation = abbreviation ? reinterpret_cast<const char*>(abbreviation) : "";
        
        const unsigned char* logo_path = sqlite3_column_text(stmt, 3);
        team.logo_path = logo_path ? reinterpret_cast<const char*>(logo_path) : "";
        
        const unsigned char* conference = sqlite3_column_text(stmt, 4);
        team.conference = conference ? reinterpret_cast<const char*>(conference) : "";
        
        const unsigned char* division = sqlite3_column_text(stmt, 5);
        team.division = division ? reinterpret_cast<const char*>(division) : "";
        
        teams.push_back(team);
    }
    
    sqlite3_finalize(stmt);
    return teams;
}

DatabaseManager::TeamData DatabaseManager::getTeamById(const std::string& teamId) {
    TeamData team;
    const char* query = "SELECT id, name, abbreviation, logo_path, conference, division FROM teams WHERE id = ?";
    
    sqlite3_stmt* stmt;
    checkError(sqlite3_prepare_v2(db, query, -1, &stmt, nullptr), "prepare team by ID statement");
    
    checkError(sqlite3_bind_text(stmt, 1, teamId.c_str(), -1, SQLITE_STATIC), "bind team ID");
    
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        const unsigned char* id = sqlite3_column_text(stmt, 0);
        team.id = id ? reinterpret_cast<const char*>(id) : "";
        
        const unsigned char* name = sqlite3_column_text(stmt, 1);
        team.name = name ? reinterpret_cast<const char*>(name) : "";
        
        const unsigned char* abbreviation = sqlite3_column_text(stmt, 2);
        team.abbreviation = abbreviation ? reinterpret_cast<const char*>(abbreviation) : "";
        
        const unsigned char* logo_path = sqlite3_column_text(stmt, 3);
        team.logo_path = logo_path ? reinterpret_cast<const char*>(logo_path) : "";
        
        const unsigned char* conference = sqlite3_column_text(stmt, 4);
        team.conference = conference ? reinterpret_cast<const char*>(conference) : "";
        
        const unsigned char* division = sqlite3_column_text(stmt, 5);
        team.division = division ? reinterpret_cast<const char*>(division) : "";
    } else {
        throw std::runtime_error("Team not found");
    }
    
    sqlite3_finalize(stmt);
    return team;
} 