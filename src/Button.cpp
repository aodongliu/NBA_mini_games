// Button.cpp
#include "Button.hpp"

Button::Button() {
    buttonShape.setSize(sf::Vector2f(0, 0));
    buttonShape.setFillColor(sf::Color::Transparent);
    buttonShape.setOutlineColor(sf::Color::Transparent);
    buttonShape.setOutlineThickness(0);

    buttonText.setString("");
    buttonText.setCharacterSize(0);
}

Button::Button(const sf::Font& font, const std::string& text, const sf::Vector2f& position, 
               const sf::Vector2f& size, const sf::Color& bgColor, const sf::Color& textColor) {
    buttonShape.setPosition(position);
    buttonShape.setSize(size);
    buttonShape.setFillColor(bgColor);
    buttonShape.setOutlineThickness(2);

    buttonText.setFont(font);
    buttonText.setString(text);
    buttonText.setCharacterSize(14); // Default font size
    buttonText.setFillColor(textColor);
    buttonText.setPosition(
        position.x + (size.x - buttonText.getLocalBounds().width) / 2,
        position.y + (size.y - buttonText.getLocalBounds().height) / 2
    );
}

void Button::render(sf::RenderWindow& window) {
    window.draw(buttonShape);
    window.draw(buttonText);
}

bool Button::isHovered(const sf::Vector2i& mousePos) const {
    return buttonShape.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos));
}

bool Button::isClicked(const sf::Event& event, const sf::Vector2i& mousePos) const {
    return (event.type == sf::Event::MouseButtonPressed &&
            buttonShape.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)));
}

bool Button::isDoubleClicked(const sf::Event& event, const sf::Vector2i& mousePos) {
    if (isClicked(event, mousePos)) {
        if (doubleClickClock.getElapsedTime().asSeconds() < 0.3f) {
            doubleClickFlag = true;  // Double click detected
        } else {
            doubleClickFlag = false; // Single click
        }
        doubleClickClock.restart(); // Restart the timer for the next click
    }
    return doubleClickFlag;
}

void Button::resetDoubleClickFlag() {
    doubleClickFlag = false;
}

void Button::setTheme(const sf::Color& bgColor, const sf::Color& textColor, const sf::Color& borderColor) {
    buttonShape.setFillColor(bgColor);
    buttonShape.setOutlineColor(borderColor);
    buttonText.setFillColor(textColor);
}

void Button::setHighlightTheme(const ThemeConfig& themeConfig) {
    setTheme(themeConfig.highlightAreaColor, themeConfig.highlightTextColor, themeConfig.borderColor);
}

void Button::setDefaultTheme(const ThemeConfig& themeConfig) {
    setTheme(themeConfig.buttonColor, themeConfig.instructionTextColor, themeConfig.borderColor);
}

