#include "MainMenu.hpp"

MainMenu::MainMenu(const sf::Font& font, const sf::Vector2u& windowSize)
    : selectedOption(-1) {

    sf::Text playGame("Play Ranking Game", font, 30);
    playGame.setPosition(windowSize.x/2-150, windowSize.y/3);
    options.push_back(playGame);

    sf::Text quit("Quit", font, 30);
    quit.setPosition(windowSize.x/2-50, windowSize.y/2);
    options.push_back(quit);
}


void MainMenu::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed) {
        for (size_t i = 0; i < options.size(); ++i){
            if (options[i].getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                selectedOption = i;
            }
        }
    }
}

void MainMenu::render(sf::RenderWindow& window) {
    for (const auto& option : options) {
        window.draw(option);
    }
}

int MainMenu::getSelectedOption() const {
    return selectedOption;
}