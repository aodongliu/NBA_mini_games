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
               const sf::Vector2f& size, const sf::Color& bgColor, const sf::Color& textColor) 
        : defaultBgColor(bgColor), defaultTextColor(textColor) {
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

bool Button::updateHoverState(const sf::Vector2i& mousePos, const sf::Color& hoverBgColor, const sf::Color& hoverTextColor) {
    bool isHovered = buttonShape.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos));
    if (buttonShape.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
        buttonShape.setFillColor(hoverBgColor);
        buttonText.setFillColor(hoverTextColor);
    } else {
        // Reset to default colors
        buttonShape.setFillColor(defaultBgColor);
        buttonText.setFillColor(defaultTextColor);
    }
    return isHovered;
}

bool Button::isClicked(const sf::Event& event, const sf::Vector2i& mousePos) const {
    return (event.type == sf::Event::MouseButtonPressed &&
            buttonShape.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)));
}

void Button::setTheme(const sf::Color& bgColor, const sf::Color& textColor, const sf::Color& borderColor) {
    buttonShape.setFillColor(bgColor);
    buttonShape.setOutlineColor(borderColor);
    buttonText.setFillColor(textColor);
}
