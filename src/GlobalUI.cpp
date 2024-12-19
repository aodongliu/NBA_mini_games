#include "GlobalUI.hpp"

GlobalUI::GlobalUI(const sf::Font& font, const sf::Vector2u& windowSize) {
    setupThemeButtons(font, windowSize);
}

void GlobalUI::setupThemeButtons(const sf::Font& font, const sf::Vector2u& windowSize) {
    float buttonWidth = 80;
    float buttonHeight = 30;
    float padding = 10;

    lightButtonPos = sf::Vector2f(windowSize.x - 2 * (buttonWidth + padding), padding);
    darkButtonPos = sf::Vector2f(windowSize.x - (buttonWidth + padding), padding);

    const ThemeConfig& lightConfig = ThemeManager::getInstance().getLightThemeConfig();
    lightButton = Button(font, "Light", lightButtonPos, sf::Vector2f(buttonWidth, buttonHeight),
                         lightConfig.buttonColor, lightConfig.instructionTextColor);

    const ThemeConfig& darkConfig = ThemeManager::getInstance().getDarkThemeConfig();
    darkButton = Button(font, "Dark", darkButtonPos, sf::Vector2f(buttonWidth, buttonHeight),
                        darkConfig.buttonColor, darkConfig.instructionTextColor);
}

void GlobalUI::updateSize(const sf::Font& font, const sf::Vector2u& windowSize) {
    setupThemeButtons(font, windowSize);
}

void GlobalUI::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

    // Handle hover states
    if (lightButton.isHovered(mousePos)) {
        lightButton.setHighlightTheme(ThemeManager::getInstance().getLightThemeConfig());
    } else {
        lightButton.setDefaultTheme(ThemeManager::getInstance().getLightThemeConfig());
    }

    if (darkButton.isHovered(mousePos)) {
        darkButton.setHighlightTheme(ThemeManager::getInstance().getDarkThemeConfig());
    } else {
        darkButton.setDefaultTheme(ThemeManager::getInstance().getDarkThemeConfig());
    }

    // Handle button clicks
    if (lightButton.isClicked(event, mousePos)) {
        ThemeManager::getInstance().toggleTheme(Theme::Light);
    } else if (darkButton.isClicked(event, mousePos)) {
        ThemeManager::getInstance().toggleTheme(Theme::Dark);
    }
}

void GlobalUI::render(sf::RenderWindow& window) {
    lightButton.render(window);
    darkButton.render(window);
}
