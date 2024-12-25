#pragma once

#include <SFML/Graphics.hpp>
#include <utils/ThemeManager.hpp>
#include <base/enums.hpp>
//#include <ui/TextManager.hpp>
#include <ui/Label.hpp>

class WindowBase {
public:
    WindowBase() = delete;
    WindowBase(sf::RenderWindow& window);
    virtual ~WindowBase() = default;

    virtual void handleEvent(const sf::Event& event) = 0;
    virtual void render(sf::RenderWindow& window) = 0;
    virtual void updateTheme() = 0;

protected:
    sf::RenderWindow& window;
    sf::Vector2u windowSize; 

};