#pragma once
#include <ThemeManager.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class MainMenu {

public:
    MainMenu(const sf::Font& font, const sf::Vector2u& windowSize);

    void handleEvent(const sf::Event& event);
    void render(sf::RenderWindow& window);
    int  getSelectedOption() const;
    bool isOptionDoubleClicked();

private:
    std::vector<sf::Text> menuOptions;
    int selectedOption;
    sf::Clock doubleClickClock;
    bool doubleClickFlag;

};