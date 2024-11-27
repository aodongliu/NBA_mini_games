#ifndef PLAYER_MANAGER_HPP
#define PLAYER_MANAGER_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <map>

class PlayerManager {
public:
    PlayerManager(const std::string& playerImagesDir, size_t maxPlayers);
    bool loadNextPlayer(sf::Sprite& sprite, sf::Texture& texture, sf::Vector2u windowSize);
    bool assignRank(int rank, sf::Sprite& rankedSprite);
    bool isRankingComplete() const;
    std::map<int, sf::Sprite> getRankingSprites() const;
    std::map<int, std::string> getRankingNames() const; // Get names associated with ranks

private:
    std::vector<std::string> playerImages;
    std::map<int, sf::Sprite> rankingSprites;
    std::map<int, std::string> rankingNames; // Map ranks to player names
    size_t currentPlayerIndex = 0;
    size_t maxPlayers;

    sf::Sprite createScaledSprite(const std::string& imagePath, float scale);
};

#endif // PLAYER_MANAGER_HPP

