#include "Player.hpp"
#include "config.hpp"
#include <sstream>
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
