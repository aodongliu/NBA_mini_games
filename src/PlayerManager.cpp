#include "PlayerManager.hpp"
#include <filesystem>
#include <iostream>
#include <random> // Add this for random_device, mt19937, and shuffle

namespace fs = std::filesystem;

PlayerManager::PlayerManager(const std::string& playerImagesDir, size_t maxPlayers) : maxPlayers(maxPlayers) {
    for (const auto& entry : fs::directory_iterator(playerImagesDir)) {
        if (entry.path().extension() == ".jpg") {
            playerImages.push_back(entry.path().string());
        }
    }
    if (playerImages.empty()) {
        throw std::runtime_error("Error: No images found in directory " + playerImagesDir);
    }

    // Randomize the playerImages vector
    std::random_device rd; // Seed for the random number generator
    std::mt19937 g(rd());  // Mersenne Twister generator
    std::shuffle(playerImages.begin(), playerImages.end(), g); // Shuffle the vector
    playerImages.resize(maxPlayers); // Keep only maxPlayers
}

bool PlayerManager::loadNextPlayer(sf::Sprite& sprite, sf::Texture& texture, sf::Vector2u windowSize) {
    if (currentPlayerIndex >= playerImages.size()) return false;

    if (!texture.loadFromFile(playerImages[currentPlayerIndex])) {
        std::cerr << "Error: Failed to load image " << playerImages[currentPlayerIndex] << "\n";
        return false;
    }
    sprite.setTexture(texture);
    sprite.setPosition((windowSize.x / 2) - (texture.getSize().x / 2), (windowSize.y / 2) - (texture.getSize().y / 2));
    currentPlayerIndex++;
    return true;
}

bool PlayerManager::assignRank(int rank, sf::Sprite& rankedSprite) {
    if (rank < 1 || rank > maxPlayers || rankingSprites.count(rank)) return false;

    rankedSprite = createScaledSprite(playerImages[currentPlayerIndex - 1], 0.25f);
    rankingSprites[rank] = rankedSprite;
    rankingNames[rank] = fs::path(playerImages[currentPlayerIndex - 1]).stem().string(); // Store player name
    return true;
}

bool PlayerManager::isRankingComplete() const {
    return rankingSprites.size() == maxPlayers;
}

std::map<int, sf::Sprite> PlayerManager::getRankingSprites() const {
    return rankingSprites;
}

std::map<int, std::string> PlayerManager::getRankingNames() const {
    return rankingNames;
}

sf::Sprite PlayerManager::createScaledSprite(const std::string& imagePath, float scale) {
    sf::Texture* texture = new sf::Texture();
    if (!texture->loadFromFile(imagePath)) {
        throw std::runtime_error("Error: Failed to load image " + imagePath);
    }
    sf::Sprite sprite(*texture);
    sprite.setScale(scale, scale);
    return sprite;
}

