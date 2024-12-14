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

    std::string userInput;

    sf::Text instructionText;
    sf::Sprite currentPlayerSprite;
    sf::Texture currentPlayerTexture;

    std::vector<Player> players;
    std::map<int, std::shared_ptr<Player>> rankings;
    size_t currentPlayerIndex;
    
    std::string errorMessage;
    sf::Clock errorClock;


    void drawText(sf::RenderWindow& window, const std::string& content, 
        const sf::Font& font, unsigned int size, sf::Vector2f position, sf::Color color) const;

    void setInstruction(const std::string& message);
    void setError(const std::string& message);

    bool isValidInput(const std::string& input, int& rank, std::string& errorMsg);
    void loadNextPlayer();
    void displayRankings(sf::RenderWindow& window);
    void saveRankingToCSV();
    void clearRankings();
};
