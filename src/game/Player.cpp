#include "game/Player.hpp"
#include "config.hpp"
#include <sstream>
#include <fstream>
#include <filesystem>

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
        throw ErrorHandling::FileError("Player image not found: " + imagePath);
    }

}

bool Player::loadImage(sf::Texture& texture) const {
    try {
        auto& resourceManager = ResourceManager::getInstance();
        auto loadedTexture = resourceManager.loadTexture(imagePath);
        texture = *loadedTexture;
        return true;
    } catch (const ErrorHandling::ResourceError& e) {
        std::cerr << e.what() << std::endl;
        return false;
    }
}

std::vector<Player> Player::loadPlayersFromCSV() {
    std::vector<Player> players;
    std::ifstream file(PLAYER_LIST_PATH);

    if(!file.is_open()){
        throw ErrorHandling::FileError("Unable to open player CSV file: " + std::string(PLAYER_LIST_PATH));
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
