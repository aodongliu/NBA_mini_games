#include "game/Player.hpp"
#include <stdexcept>

Player::Player(const DatabaseManager::PlayerRecord& record)
    : id(record.id)
    , first_name(record.first_name)
    , last_name(record.last_name)
    , team_id(record.team_id)
    , headshot_path(record.headshot_path)
    , position(record.position)
    , height(record.height)
    , weight(record.weight)
    , points_per_game(record.points_per_game)
    , rebounds_per_game(record.rebounds_per_game)
    , assists_per_game(record.assists_per_game)
    , field_goal_percentage(record.field_goal_percentage)
    , three_point_percentage(record.three_point_percentage) {
}

std::vector<Player> Player::getPlayersAbovePoints(float minPPG) {
    std::vector<Player> players;
    auto& db = DatabaseManager::getInstance();
    
    std::string whereClause = "points_per_game >= ?";
    auto records = db.getPlayers(whereClause, {std::to_string(minPPG)});
    
    for (const auto& record : records) {
        players.emplace_back(record);
    }
    return players;
}

std::vector<Player> Player::getPlayersByTeam(const std::string& teamId) {
    std::vector<Player> players;
    auto& db = DatabaseManager::getInstance();
    auto records = db.getPlayersByTeam(teamId);
    
    for (const auto& record : records) {
        players.emplace_back(record);
    }
    return players;
}

Player Player::getPlayerById(int playerId) {
    auto& db = DatabaseManager::getInstance();
    auto record = db.getPlayerById(playerId);
    return Player(record);
}
