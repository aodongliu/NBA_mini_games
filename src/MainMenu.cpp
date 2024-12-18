#include "MainMenu.hpp"

MainMenu::MainMenu(const sf::Font& font, const sf::Vector2u& windowSize)
    : font(font), windowSize(windowSize), selectedOption(0), doubleClickFlag(false) {
    menuOptions = {"Play Ranking Game", "Quit"};
}


void MainMenu::handleEvent(const sf::Event& event) {
    const ThemeConfig& themeConfig = ThemeManager::getInstance().getThemeConfig();

    // Handle keyboard navigation
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Up) {
            selectedOption = (selectedOption - 1 + menuOptions.size()) % menuOptions.size();
        } else if (event.key.code == sf::Keyboard::Down) {
            selectedOption = (selectedOption + 1) % menuOptions.size();
        }
    }

    // Handle mouse hover
    if (event.type == sf::Event::MouseMoved) {
        sf::Vector2f mousePos(static_cast<float>(event.mouseMove.x), static_cast<float>(event.mouseMove.y));
        for (size_t i = 0; i < menuOptions.size(); ++i) {
            sf::Text tempText;
            tempText.setFont(font);
            tempText.setString(menuOptions[i]);
            tempText.setCharacterSize(30); // Match rendering size
            tempText.setFillColor(themeConfig.instructionTextColor);
            tempText.setPosition(
                TextManager::getXCenter(menuOptions[i], font, 30, windowSize.x),
                windowSize.y / 3.0f + i * 50 // Y offset for each option
            );

            if (tempText.getGlobalBounds().contains(mousePos)) {
                selectedOption = i;
                break;
            }
        }
    }

    // Handle mouse clicks
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
        sf::Text tempText;
        tempText.setFont(font);
        tempText.setString(menuOptions[selectedOption]);
        tempText.setCharacterSize(30);
        tempText.setFillColor(themeConfig.instructionTextColor);
        tempText.setPosition(
            TextManager::getXCenter(menuOptions[selectedOption], font, 30, windowSize.x),
            windowSize.y / 3.0f + selectedOption * 50
        );

        if (tempText.getGlobalBounds().contains(mousePos)) {
            if (doubleClickClock.getElapsedTime().asSeconds() < 0.3f) {
                doubleClickFlag = true; // Double click detected
            } else {
                doubleClickFlag = false;
            }
            doubleClickClock.restart();
        }
    }
}

void MainMenu::render(sf::RenderWindow& window) {
    const ThemeConfig& theme = ThemeManager::getInstance().getThemeConfig();
    float yOffset = windowSize.y / 3.0f;

    for (size_t i = 0; i < menuOptions.size(); ++i) {
        sf::Color color = (i == selectedOption) ? theme.highlightTextColor : theme.instructionTextColor;
        float xCenter = TextManager::getXCenter(menuOptions[i], font, 30, windowSize.x);
        TextManager::drawText(window, menuOptions[i], font, 30, sf::Vector2f(xCenter, yOffset), color);
        yOffset += 50;
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