#pragma once
#include <ui/Button.hpp>
//#include <ui/TextManager.hpp>
#include <utils/ThemeManager.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class MainMenu {

public:
    MainMenu(const sf::Font& font, const sf::Vector2u& windowSize);

    void addOption(const std::string& optionText, std::function<void()> callback);
    void reset();
    void handleEvent(const sf::Event& event, sf::RenderWindow& window);
    void render(sf::RenderWindow& window);

private:
    std::vector<Button> menuButtons;
    const sf::Font& font;
    sf::Vector2u windowSize;

    int selectedOption;
    bool selectedOptionTriggered;
    void updateButtonThemes();
};