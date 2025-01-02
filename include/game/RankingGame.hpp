#pragma once

#include "base/GameBase.hpp"
#include "game/Player.hpp"
#include <vector>
#include <map>
#include <memory>
class RankingGame : public GameBase {

public:
    RankingGame(sf::RenderWindow& window, std::shared_ptr<sf::Font> font);

    void handleEvent(const sf::Event& event) override;
    void render(sf::RenderWindow& window) override;
    void resetGame() override;
    void updateTheme() override;
    
private:

    sf::Sprite currentPlayerSprite;
    sf::Texture currentPlayerTexture;
    Label inputLabel;

    std::string userInput;
    std::map<int, std::shared_ptr<Player>> rankings;
    size_t currentPlayerIndex;
    
    void setUpLabels() override;
    bool isValidInput(const std::string& input, int& rank, std::string& errorMsg);
    void configurePlayerSprite();
    void loadNextPlayer();
    void displayRankings(sf::RenderWindow& window);
    void saveRankingToCSV();
};
