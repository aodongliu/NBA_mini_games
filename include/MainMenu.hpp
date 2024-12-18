#pragma once
#include <ThemeManager.hpp>
#include <SFML/Graphics.hpp>
#include <TextManager.hpp>
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
    std::vector<std::string> menuOptions;
    int selectedOption;
    sf::Clock doubleClickClock;
    bool doubleClickFlag;

    const sf::Font& font;
    sf::Vector2u windowSize;

};