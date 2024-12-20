#pragma once

#include "base/GameBase.hpp"
#include "game/Player.hpp"
#include <vector>
#include <map>
#include <memory>
class RankingGame : public GameBase {

public:
    RankingGame(const sf::Font& font, sf::Vector2u windowSize);

    void handleEvent(const sf::Event& event) override;

    void render(sf::RenderWindow& window) override;
    
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
    
    std::string instructionMessage;
    std::string errorMessage;
    sf::Clock errorClock;

    void setInstruction(const std::string& message);
    void setError(const std::string& message);

    bool isValidInput(const std::string& input, int& rank, std::string& errorMsg);
    void loadNextPlayer();
    void displayRankings(sf::RenderWindow& window);
    void saveRankingToCSV();
};
