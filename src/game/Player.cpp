#include "game/Player.hpp"
#include "config.hpp"
#include <sstream>
#include <fstream>
#include <filesystem>
#include <stdexcept>

namespace fs = std::filesystem;

Player::Player(const std::string& firstName, const std::string& lastName)
    : firstName(firstName), lastName(lastName) {
    constructImagePath();
}

void Player::constructImagePath(){

    std::ostringstream pathStream;
    pathStream << PLAYER_IMAGES_PATH << "/" << firstName << "-" << lastName << ".jpg";

    imagePath = pathStream.str();
    
    if (!fs::exists(imagePath)) {
        throw std::runtime_error("Image file not found: " + imagePath);
    }

}

bool Player::loadImage(sf::Texture& texture) const{
    return texture.loadFromFile(imagePath);
}

std::vector<Player> Player::loadPlayersFromCSV() {
    std::vector<Player> players;
    std::ifstream file(PLAYER_LIST_PATH);

    if(!file.is_open()){
        throw std::runtime_error("Error: Unable to open CSV file at " + std::string(PLAYER_LIST_PATH));
    }
    
    std::string line;
    std::getline(file, line);

    while (std::getline(file, line)){
        if (!line.empty() && line.back() == '\r')
            line.pop_back();
        std::istringstream ss(line);
        std::string firstName, lastName;

        if (std::getline(ss, firstName, ',') && std::getline(ss, lastName, ',')) {
            players.emplace_back(firstName, lastName);
        }
    }
  
    file.close();
    return players;
}
