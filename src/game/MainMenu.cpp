#include "game/MainMenu.hpp"

MainMenu::MainMenu(sf::RenderWindow& window, std::shared_ptr<sf::Font> font)
    : WindowBase(window), font(font), selectedOption(0), selectedOptionTriggered(false) {}

void MainMenu::addOption(const std::string& optionText, std::function<void()> callback) { 

    const ThemeConfig& themeConfig = ThemeManager::getInstance().getThemeConfig();
    float buttonWidth = 200.0f;
    float buttonHeight = 50.0f; 
    float yOffset = windowSize.y / 3.0f;
    sf::Vector2f position(
        (windowSize.x - buttonWidth) / 2, // Center horizontally
        yOffset + menuButtons.size() * (buttonHeight + 10) // Adjust vertical position
    );

    Button button(font, optionText, position, {buttonWidth, buttonHeight}, 
                  themeConfig.buttonColor, themeConfig.instructionTextColor);
    button.setCallback(callback);
    menuButtons.push_back(button);
}

void MainMenu::reset() {
    selectedOption = 0;
    selectedOptionTriggered = false; 
    for (auto& button : menuButtons) {
        button.resetDoubleClickFlag();
    }
}

void MainMenu::handleEvent(const sf::Event& event) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window); // Use relative position to the window

    // Handle mouse hover
    for (size_t i = 0; i < menuButtons.size(); ++i) {
        if (menuButtons[i].isHovered(mousePos) && event.type == sf::Event::MouseMoved) {
            selectedOption = i;
        }

        if (menuButtons[i].isDoubleClicked(event, mousePos)) {
            menuButtons[i].triggerCallback();
        }
    }

    // Handle keyboard navigation
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Up) {
            selectedOption = (selectedOption - 1 + menuButtons.size()) % menuButtons.size();
        } else if (event.key.code == sf::Keyboard::Down) {
            selectedOption = (selectedOption + 1) % menuButtons.size();
        } else if (event.key.code == sf::Keyboard::Enter) {
            menuButtons[selectedOption].triggerCallback();
        }
    }
}

void MainMenu::render(sf::RenderWindow& window) {
    for (auto& button : menuButtons) {
        button.render(window);
    }
}

void MainMenu::updateButtonThemes() {
    const ThemeConfig& themeConfig = ThemeManager::getInstance().getThemeConfig();
    for (size_t i = 0; i < menuButtons.size(); ++i) {
        if (i == selectedOption) {
            menuButtons[i].setHighlightTheme(themeConfig);
        } else {
            menuButtons[i].setDefaultTheme(themeConfig);
        }
    }
}

void MainMenu::updateTheme() {
    updateButtonThemes();
}


