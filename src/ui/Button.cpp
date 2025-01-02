#include "ui/Button.hpp"

Button::Button() {
    buttonShape.setSize(sf::Vector2f(0, 0));
    buttonShape.setFillColor(sf::Color::Transparent);
    buttonShape.setOutlineColor(sf::Color::Transparent);
    buttonShape.setOutlineThickness(0);

    buttonText.setString("");
    buttonText.setCharacterSize(0);
}

Button::Button(std::shared_ptr<sf::Font> font, const std::string& text, const sf::Vector2f& position, 
               const sf::Vector2f& size, const sf::Color& bgColor, const sf::Color& textColor)
        : Button() {
    buttonShape.setPosition(position);
    buttonShape.setSize(size);
    buttonShape.setFillColor(bgColor);
    buttonShape.setOutlineThickness(2);

    buttonText.setFont(*font);
    buttonText.setString(text);
    buttonText.setCharacterSize(14); // Default font size
    buttonText.setFillColor(textColor);
    centerText();
}

void Button::setCallback(std::function<void()> onClickCallback) {
    onClick = onClickCallback;
}

void Button::render(sf::RenderWindow& window) {
    if (!isVisible()) return;
    window.draw(buttonShape);
    window.draw(buttonText);
}

void Button::centerText() {
    sf::FloatRect textBounds = buttonText.getLocalBounds();
    sf::FloatRect shapeBounds = buttonShape.getGlobalBounds();
    buttonText.setPosition(
        shapeBounds.left + (shapeBounds.width - textBounds.width) / 2 - textBounds.left,
        shapeBounds.top + (shapeBounds.height - textBounds.height) / 2 - textBounds.top
    );
}

bool Button::isHovered(const sf::Vector2i& mousePos) const {
    return buttonShape.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos));
}

bool Button::isClicked(const sf::Event& event, const sf::Vector2i& mousePos) const {
    return (event.type == sf::Event::MouseButtonPressed &&
            buttonShape.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)));
}

bool Button::isDoubleClicked(const sf::Event& event, const sf::Vector2i& mousePos) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        if (buttonShape.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
            if (doubleClickClock.getElapsedTime().asSeconds() < 0.3f) {
                doubleClickFlag = true; // Double click detected
            } else {
                doubleClickFlag = false; // Single click
            }
            doubleClickClock.restart();
        }
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

void Button::triggerCallback() {
    if (onClick) {
        onClick();
    }
}

