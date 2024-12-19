#pragma once
#include <Button.hpp>
#include <ThemeManager.hpp>
#include <SFML/Graphics.hpp>
#include <TextManager.hpp>
#include <vector>
#include <string>

class MainMenu {

public:
    MainMenu(const sf::Font& font, const sf::Vector2u& windowSize);

    void reset();
    void handleEvent(const sf::Event& event, sf::RenderWindow& window);
    void render(sf::RenderWindow& window);
    std::optional<int> handleAction();

private:
    std::vector<Button> menuButtons;
    const sf::Font& font;
    sf::Vector2u windowSize;

    std::vector<std::string> options = {"Play Ranking Game", "Quit"};
    int selectedOption;
    bool selectedOptionTriggered;
};