#pragma once

#include "base/WindowBase.hpp"
#include "game/Player.hpp"
#include "utils/DatabaseManager.hpp"
#include <vector>
#include <memory>

class GameBase : public WindowBase {

public:
    GameBase(sf::RenderWindow& window, std::shared_ptr<sf::Font> font);
    virtual ~GameBase() = default;

    virtual void handleEvent(const sf::Event& event) = 0;
    virtual void render(sf::RenderWindow& window) = 0;
    virtual void resetGame() = 0;
    virtual void updateTheme() = 0;

    GameState GameState;

protected:
    std::vector<Player> players;
    size_t numPlayers;
    std::shared_ptr<sf::Font> font;
    Label instructionLabel;
    Label errorLabel;
    sf::Clock errorClock;
    bool quitConfirmation;

    virtual void setUpLabels() = 0;
    void loadPlayers(size_t count);
};
