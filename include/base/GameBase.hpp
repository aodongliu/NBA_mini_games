#pragma once

#include "base/WindowBase.hpp"
#include "game/Player.hpp"
#include <vector>
#include <map>
#include <memory>
class GameBase : public WindowBase {

public:
    GameBase(sf::RenderWindow& window, const sf::Font& font);
    virtual ~GameBase() = default;

    virtual void handleEvent(const sf::Event& event) = 0;
    virtual void render(sf::RenderWindow& window) = 0;
    virtual void resetGame() = 0;
    virtual void updateTheme();

    SubGameState subGameState;

protected:

    const sf::Font& font;
    Label instructionLabel;
    Label errorLabel;
    Label inputLabel;
    sf::Clock errorClock;

    bool quitConfirmation;
    std::vector<Player> players;
    
    virtual void setUpLabels() = 0;
    virtual void loadRandomPlayers(size_t numPlayers);
};
