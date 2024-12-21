#pragma once

#include "base/GameBase.hpp"
#include "game/Player.hpp"
#include <vector>
#include <map>
#include <memory>
class RankingGame : public GameBase {

public:
    RankingGame(sf::RenderWindow& window, const sf::Font& font);

    void handleEvent(const sf::Event& event) override;
    void render(sf::RenderWindow& window) override;
    void updateTheme() override;
    
    void resetGame();
    
    SubGameState subGameState;

private:

    const sf::Font& font;

    bool quitConfirmation;

    std::string userInput;

    sf::Sprite currentPlayerSprite;
    sf::Texture currentPlayerTexture;

    std::vector<Player> players;
    std::map<int, std::shared_ptr<Player>> rankings;
    size_t currentPlayerIndex;
    
    Label instructionLabel;
    Label errorLabel;
    Label inputLabel;
    sf::Clock errorClock;

    bool isValidInput(const std::string& input, int& rank, std::string& errorMsg);
    void configurePlayerSprite();
    void setUpLabels();
    void loadRandomPlayers();
    void loadNextPlayer();
    void displayRankings(sf::RenderWindow& window);
    void saveRankingToCSV();
};
