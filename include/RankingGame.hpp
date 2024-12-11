#pragma once

#include "GameBase.hpp"
#include "Player.hpp"
#include <vector>
#include <map>
#include <memory>
class RankingGame : public GameBase {

public:
    RankingGame(const sf::Font& font, sf::Vector2u windowSize);

    void handleEvent(const sf::Event& event) override;

    void render(sf::RenderWindow& window) override;

private:

    sf::Text instructionText;
    sf::Sprite currentPlayerSprite;
    sf::Texture currentPlayerTexture;

    std::vector<Player> players;
    std::map<int, std::shared_ptr<Player>> rankings;
    size_t currentPlayerIndex;

    void loadNextPlayer();
    void displayRankings(sf::RenderWindow& window);
    void saveRankingToCSV();
};
