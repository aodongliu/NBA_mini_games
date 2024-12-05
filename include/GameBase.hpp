#pragma once

#include <SFML/Graphics.hpp>

class GameBase {
public:
    virtual ~GameBase() = default;

    virtual void handleEvent(const sf::Event& event) = 0;
    virtual void render(sf::RenderWindow& window) = 0;

};