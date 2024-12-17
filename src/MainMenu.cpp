#include "MainMenu.hpp"

MainMenu::MainMenu(const sf::Font& font, const sf::Vector2u& windowSize)
    : selectedOption(0), doubleClickFlag(false) {
    
    const ThemeConfig& themeConfig = ThemeManager::getInstance().getThemeConfig();

    // TODO: refactor this in a container
    std::vector<std::string> options = {"Play Ranking Game", "Quit"}; 

    float yOffset = windowSize.y/3;

    for (size_t i = 0; i < options.size(); ++i){
        sf::Text option;
        option.setFont(font);
        option.setString(options[i]);
        option.setCharacterSize(30);
        option.setFillColor(i==selectedOption? themeConfig.highlightTextColor : themeConfig.instructionTextColor);
        option.setPosition(windowSize.x/2 - option.getLocalBounds().width/2,yOffset);
        yOffset += 50;
        menuOptions.push_back(option);
    }

}


void MainMenu::handleEvent(const sf::Event& event) {
    
    const ThemeConfig& themeConfig= ThemeManager::getInstance().getThemeConfig();
    
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Up) {
            selectedOption = (selectedOption - 1 + menuOptions.size()) % menuOptions.size();
        } else if (event.key.code == sf::Keyboard::Down) {
            selectedOption = (selectedOption + 1) % menuOptions.size();
        }
    } 
    
    if (event.type == sf::Event::MouseMoved) {
        for (size_t i = 0; i < menuOptions.size(); ++i) {
            if (menuOptions[i].getGlobalBounds().contains(event.mouseMove.x, event.mouseMove.y)) {
                selectedOption = i; 
            }
        }
    }
    
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        if (menuOptions[selectedOption].getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
            if (doubleClickClock.getElapsedTime().asSeconds() < 0.3f) {
                doubleClickFlag = true;
            } else {
                doubleClickFlag = false;
            } 
            doubleClickClock.restart();
        }
    }
    
    for (size_t i = 0; i < menuOptions.size(); ++i) {
        menuOptions[i].setFillColor( i==selectedOption ? themeConfig.highlightTextColor : themeConfig.instructionTextColor);
    }
}

void MainMenu::render(sf::RenderWindow& window) {
    for (const auto& option : menuOptions) {
        window.draw(option);
    }
}

int MainMenu::getSelectedOption() const {
    return selectedOption;
}

bool MainMenu::isOptionDoubleClicked() {
    if (doubleClickFlag) {
        doubleClickFlag = false; // Reset the flag after checking
        return true;
    }
    return false;
}