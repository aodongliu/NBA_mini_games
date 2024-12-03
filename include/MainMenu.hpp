#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class MainMenu {

public:
    MainMenu(const sf::Font& font, const sf::Vector2u& windowSize);

    void handleEvent(const sf::Event& event);
    void render(sf::RenderWindow& window);

    int getSelectedOption() const;

private:
    std::vector<sf::Text> options;
    int selectedOption;

};