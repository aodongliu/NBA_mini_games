#pragma once

#include <SFML/Graphics.hpp>
#include "Button.hpp"
#include "ThemeManager.hpp"

class GlobalUI {
public:
    GlobalUI(const sf::Font& font, const sf::Vector2u& windowSize);

    void handleEvent(const sf::Event& event, const sf::RenderWindow& window);
    void render(sf::RenderWindow& window);
    void updateSize(const sf::Font& font, const sf::Vector2u& windowSize);

private:
    Button lightButton, darkButton;
    sf::Vector2f lightButtonPos, darkButtonPos;

    void setupThemeButtons(const sf::Font& font, const sf::Vector2u& windowSize);

};