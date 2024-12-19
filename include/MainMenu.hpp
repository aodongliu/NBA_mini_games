#pragma once
#include <ThemeManager.hpp>
#include <SFML/Graphics.hpp>
#include <TextManager.hpp>
#include <vector>
#include <string>

class MainMenu {

public:
    MainMenu(const sf::Font& font, const sf::Vector2u& windowSize);

    void handleEvent(const sf::Event& event, sf::RenderWindow& window);
    void render(sf::RenderWindow& window);
    int  getSelectedOption() const;
    bool isOptionDoubleClicked();

private:
    std::vector<Button> menuButtons;
    const sf::Font& font;
    sf::Vector2u windowSize;

    int selectedOption;
    sf::Clock doubleClickClock;
    bool doubleClickFlag;

};