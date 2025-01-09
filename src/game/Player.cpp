#include "game/Player.hpp"

Player::Player(const DatabaseManager::PlayerData& data)
    : firstName(data.firstName)
    , lastName(data.lastName)
    , teamId(data.teamId)
    , headshotPath(data.headshotPath)
    , ppg(data.ppg)
    , rpg(data.rpg)
    , apg(data.apg)
{
}
