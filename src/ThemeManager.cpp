#include "ThemeManager.hpp"

ThemeManager::ThemeManager()
    : currentTheme(Theme::Dark) {
    applyTheme(currentTheme);
}

void ThemeManager::applyTheme(Theme theme) {
    currentTheme = theme;
    if (theme == Theme::Light) {
        setLightTheme();
    } else {
        setDarkTheme();
    }
}

void ThemeManager::setLightTheme() {
    themeConfig.backgroundColor      = Colors::creamWhite;
    themeConfig.instructionTextColor = Colors::purple;
    themeConfig.highlightTextColor   = Colors::orange;
    themeConfig.warningTextColor     = Colors::red; 
    themeConfig.buttonColor          = sf::Color(240, 240, 240);
    themeConfig.borderColor          = sf::Color::Black;
}

void ThemeManager::setDarkTheme() {
    themeConfig.backgroundColor      = Colors::paleBlack; 
    themeConfig.instructionTextColor = Colors::lightGrey; 
    themeConfig.highlightTextColor   = Colors::darkYellow;  
    themeConfig.warningTextColor     = Colors::red;
    themeConfig.buttonColor          = sf::Color(70, 70, 70);
    themeConfig.borderColor          = sf::Color::White;
}

void ThemeManager::setupThemeButton(sf::RectangleShape& button, sf::Text& label, const sf::Font& font, const sf::Vector2u& windowSize) {
    button.setSize(sf::Vector2f(120, 40));
    button.setPosition(windowSize.x - 140, 10);
    button.setFillColor(themeConfig.buttonColor);
    button.setOutlineColor(themeConfig.borderColor);
    button.setOutlineThickness(2);

    label.setFont(font);
    label.setString("Theme");
    label.setCharacterSize(20);
    label.setFillColor(themeConfig.instructionTextColor);
    label.setPosition(windowSize.x - 130, 15);
}

void ThemeManager::handleThemeToggle(const sf::Event& event, const sf::Vector2i& mousePos, sf::RectangleShape& button) {
    if (event.type == sf::Event::MouseButtonPressed &&
        button.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
        // Toggle theme on button click
        Theme newTheme = currentTheme == Theme::Dark ? Theme::Light : Theme::Dark;
        applyTheme(newTheme);
    }
}

void ThemeManager::renderThemeButton(sf::RenderWindow& window, const sf::RectangleShape& button, const sf::Text& label) {
    window.draw(button);
    window.draw(label);
}