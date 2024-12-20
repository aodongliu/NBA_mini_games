#include "game/MainMenu.hpp"

MainMenu::MainMenu(const sf::Font& font, const sf::Vector2u& windowSize)
    : font(font), windowSize(windowSize), selectedOption(0) {

    const ThemeConfig& themeConfig = ThemeManager::getInstance().getThemeConfig();
    float yOffset = windowSize.y / 3.0f;
    float buttonWidth = 200.0f;
    float buttonHeight = 50.0f; 

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
        if (menuButtons[i].isHovered(mousePos) && event.type == sf::Event::MouseMoved) {
            selectedOption = i; // Update selected option on hover
        }
    }

    // Handle mouse double-clicks
    for (size_t i = 0; i < menuButtons.size(); ++i) {
        if (menuButtons[i].isDoubleClicked(event, mousePos)) {
            selectedOption = i; // Update the selected option
            selectedOptionTriggered = true;
        }
    }

    // Handle keyboard navigation
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Up) {
            selectedOption = (selectedOption - 1 + menuButtons.size()) % menuButtons.size();
        } else if (event.key.code == sf::Keyboard::Down) {
            selectedOption = (selectedOption + 1) % menuButtons.size();
        } else if (event.key.code == sf::Keyboard::Enter) {
            selectedOptionTriggered = true;
        }
    }
    
    // Update button themes
    for (size_t i = 0; i < options.size(); ++i) {
        if (i == selectedOption) {
            menuButtons[i].setHighlightTheme(themeConfig);
        } else {
            menuButtons[i].setDefaultTheme(themeConfig);
        }
    }
}

void MainMenu::render(sf::RenderWindow& window) {
    for (auto& button : menuButtons) {
        button.render(window);
    }
}

std::optional<int> MainMenu::handleAction() {
    if (selectedOptionTriggered) {
        selectedOptionTriggered = false; // Reset the trigger
        return selectedOption; // Return the currently selected option
    }
    return std::nullopt; // No action triggered
}

void MainMenu::reset() {
    selectedOption = 0;
    selectedOptionTriggered = false; 
    for (auto& button : menuButtons) {
        button.resetDoubleClickFlag();
    }

}
