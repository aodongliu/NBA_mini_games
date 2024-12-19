#include "MainMenu.hpp"

MainMenu::MainMenu(const sf::Font& font, const sf::Vector2u& windowSize)
    : font(font), windowSize(windowSize), selectedOption(0), doubleClickFlag(false) {

    const ThemeConfig& themeConfig = ThemeManager::getInstance().getThemeConfig();
    float yOffset = windowSize.y / 3.0f;
    float buttonWidth = 200.0f;
    float buttonHeight = 50.0f; 

    std::vector<std::string> options = {"Play Ranking Game", "Quit"};
    for (size_t i = 0; i < options.size(); ++i) {
        sf::Vector2f position(
            (windowSize.x - buttonWidth) / 2,  // Center horizontally
            yOffset + i * (buttonHeight + 10) // Adjust vertical position
        );
        menuButtons.emplace_back(
            font, options[i], position, sf::Vector2f(buttonWidth, buttonHeight),
            themeConfig.buttonColor, themeConfig.instructionTextColor
        );
    }

    // Set initial hover state for the first button
    menuButtons[selectedOption].setTheme(
        themeConfig.highlightAreaColor, themeConfig.highlightTextColor, themeConfig.borderColor
    );
}

void MainMenu::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    const ThemeConfig& themeConfig = ThemeManager::getInstance().getThemeConfig();
    sf::Vector2i mousePos = sf::Mouse::getPosition(window); // Use relative position to the window

    // Handle mouse hover
    for (size_t i = 0; i < menuButtons.size(); ++i) {
        bool isHovered = menuButtons[i].updateHoverState(
            mousePos, themeConfig.highlightAreaColor, themeConfig.highlightTextColor
        );

        if (isHovered && event.type == sf::Event::MouseMoved) {
            selectedOption = i; // Update selected option on hover
        }
    }

    // Handle mouse clicks
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        if (menuButtons[selectedOption].isClicked(event, mousePos)) {
            if (doubleClickClock.getElapsedTime().asSeconds() < 0.3f) {
                doubleClickFlag = true; // Double click detected
            } else {
                doubleClickFlag = false; // Single click
            }
            doubleClickClock.restart();
        }
    }

    // Handle keyboard navigation
    if (event.type == sf::Event::KeyPressed) {
        menuButtons[selectedOption].setTheme( // Reset previous button
            themeConfig.buttonColor, themeConfig.instructionTextColor, themeConfig.borderColor
        );

        if (event.key.code == sf::Keyboard::Up) {
            selectedOption = (selectedOption - 1 + menuButtons.size()) % menuButtons.size();
        } else if (event.key.code == sf::Keyboard::Down) {
            selectedOption = (selectedOption + 1) % menuButtons.size();
        }

        menuButtons[selectedOption].setTheme( // Highlight current button
            themeConfig.highlightAreaColor, themeConfig.highlightTextColor, themeConfig.borderColor
        );
    }
}

void MainMenu::render(sf::RenderWindow& window) {
    for (auto& button : menuButtons) {
        button.render(window);
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
