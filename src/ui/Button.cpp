#include "ui/Button.hpp"
#include <iostream>

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
    if (hasIcon) {
        window.draw(icon);
    }
    window.draw(buttonText);
}

void Button::centerText() {
    sf::FloatRect textBounds = buttonText.getLocalBounds();
    sf::FloatRect shapeBounds = buttonShape.getGlobalBounds();
    
    float xPos;
    if (hasIcon) {
        // If there's an icon, position text to the right of it
        float iconWidth = icon.getGlobalBounds().width;
        xPos = shapeBounds.left + iconWidth + 4;
    } else {
        // If no icon, center the text
        xPos = shapeBounds.left + (shapeBounds.width - textBounds.width) / 2;
    }
    
    float yPos = shapeBounds.top + (shapeBounds.height - textBounds.height) / 2;
    
    buttonText.setPosition(xPos - textBounds.left, yPos - textBounds.top);
}

bool Button::isHovered(const sf::Vector2i& mousePos) const {
    return buttonShape.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos));
}

bool Button::isClicked(const sf::Event& event, const sf::Vector2i& mousePos) const {
    return (event.type == sf::Event::MouseButtonPressed &&
            buttonShape.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)));
}

bool Button::isDoubleClicked(const sf::Event& event, const sf::Vector2i& mousePos) {
    static sf::Clock globalClickClock;
    static Button* lastClickedButton = nullptr;
    
    if (event.type == sf::Event::MouseButtonPressed && 
        event.mouseButton.button == sf::Mouse::Left) {
        
        if (buttonShape.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
            if (lastClickedButton == this && 
                globalClickClock.getElapsedTime().asSeconds() < 0.5f) {
                lastClickedButton = nullptr;
                return true;
            }
            
            lastClickedButton = this;
            globalClickClock.restart();
        }
    }
    return false;
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

void Button::setIcon(const sf::Texture& texture) {
    icon.setTexture(texture);
    
    // Scale icon to fit height of button while maintaining aspect ratio
    float scale = (buttonShape.getSize().y - 4) / texture.getSize().y;
    icon.setScale(scale, scale);
    
    // Position icon on left side of button
    icon.setPosition(
        buttonShape.getPosition().x + 2,
        buttonShape.getPosition().y + 2
    );
    
    // Adjust text position to make room for icon
    float textOffset = icon.getGlobalBounds().width + 4;
    buttonText.setPosition(
        buttonShape.getPosition().x + textOffset,
        buttonText.getPosition().y
    );
    
    hasIcon = true;

    // Remove debugging output
    // std::cout << "Icon set for button: " << buttonText.getString().toAnsiString() << std::endl;
}

