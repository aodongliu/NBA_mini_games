#pragma once

#include <SFML/Graphics.hpp>
#include "ui/Button.hpp"
#include "utils/ThemeManager.hpp"

class GlobalUI {
public:
    GlobalUI(sf::RenderWindow& window, const sf::Font& font);

    void handleEvent(const sf::Event& event);
    void render(sf::RenderWindow& window);
    void updateSize();

private:
    sf::RenderWindow& window;
    sf::Vector2u windowSize;
    sf::Font font;
       
    Button lightButton, darkButton;
    sf::Vector2f lightButtonPos, darkButtonPos;

    void setupThemeButtons();

};