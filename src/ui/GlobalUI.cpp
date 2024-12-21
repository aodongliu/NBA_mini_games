#include "ui/GlobalUI.hpp"

GlobalUI::GlobalUI(sf::RenderWindow& window, const sf::Font& font)
    : window(window), windowSize(window.getSize()), font(font) {
    setupThemeButtons();
}

void GlobalUI::setupThemeButtons() {
    float buttonWidth = 80;
    float buttonHeight = 30;
    float padding = 10;

    lightButtonPos = sf::Vector2f(windowSize.x - 2 * (buttonWidth + padding), padding);
    darkButtonPos = sf::Vector2f(windowSize.x - (buttonWidth + padding), padding);

    const ThemeConfig& lightConfig = ThemeManager::getInstance().getLightThemeConfig();
    lightButton = Button(font, "Light", lightButtonPos, sf::Vector2f(buttonWidth, buttonHeight),
                         lightConfig.buttonColor, lightConfig.instructionTextColor);
    lightButton.setCallback([this]() { ThemeManager::getInstance().toggleTheme(Theme::Light); });

    const ThemeConfig& darkConfig = ThemeManager::getInstance().getDarkThemeConfig();
    darkButton = Button(font, "Dark", darkButtonPos, sf::Vector2f(buttonWidth, buttonHeight),
                        darkConfig.buttonColor, darkConfig.instructionTextColor);
    darkButton.setCallback([this]() { ThemeManager::getInstance().toggleTheme(Theme::Dark); });
}

void GlobalUI::updateSize() {
    setupThemeButtons();
}

void GlobalUI::handleEvent(const sf::Event& event) {
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
        lightButton.triggerCallback();
    } else if (darkButton.isClicked(event, mousePos)) {
        darkButton.triggerCallback();
    }
}

void GlobalUI::render(sf::RenderWindow& window) {
    lightButton.render(window);
    darkButton.render(window);
}
