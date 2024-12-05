#pragma once

#include "GameBase.hpp"
#include "Player.hpp"
#include <vector>

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
    size_t currentPlayerIndex;

    void loadNextPlayer();
};
