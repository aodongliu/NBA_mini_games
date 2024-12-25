#pragma once

#include <SFML/Graphics.hpp>
#include <base/WindowBase.hpp>
#include <ui/GlobalUI.hpp>
#include <memory>
#include "game/MainMenu.hpp"
#include "game/RankingGame.hpp"

class GameEngine {
public:
    GameEngine(sf::RenderWindow& window, sf::Font& font);
    void run();
    void setGlobalUI(const std::shared_ptr<GlobalUI>& newGlobalUI);
    void setState(const std::shared_ptr<WindowBase>& newState);

private:
    sf::RenderWindow& window;
    std::shared_ptr<GlobalUI> globalUI;
    std::shared_ptr<WindowBase> currentState;   // Shared ownership of the active state
    std::shared_ptr<MainMenu> mainMenu;       // Shared pointer to MainMenu
    std::shared_ptr<RankingGame> rankingGame; // Shared pointer to RankingGame
};
