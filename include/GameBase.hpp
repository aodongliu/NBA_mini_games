#pragma once

#include <SFML/Graphics.hpp>
#include <ThemeManager.hpp>
#include <enums.hpp>

class GameBase {
public:
    virtual ~GameBase() = default;

    virtual void handleEvent(const sf::Event& event) = 0;
    virtual void render(sf::RenderWindow& window) = 0;

protected:
    sf::Vector2u windowSize; 

};