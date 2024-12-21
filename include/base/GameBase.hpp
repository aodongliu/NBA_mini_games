#pragma once

#include <SFML/Graphics.hpp>
#include <utils/ThemeManager.hpp>
#include <base/enums.hpp>
//#include <ui/TextManager.hpp>
#include <ui/Label.hpp>

class GameBase {
public:
    GameBase() = delete;
    GameBase(sf::RenderWindow& window);
    virtual ~GameBase() = default;

    virtual void handleEvent(const sf::Event& event) = 0;
    virtual void render(sf::RenderWindow& window) = 0;
    virtual void updateTheme() = 0;

protected:
    sf::RenderWindow& window;
    sf::Vector2u windowSize; 

};