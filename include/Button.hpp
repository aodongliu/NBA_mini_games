#pragma once
#include <SFML/Graphics.hpp>
#include "enums.hpp"
#include <string>

class Button {
public:
    Button();
    Button(const Button&) = delete;
    Button& operator=(const Button&) = delete;
    Button(Button&&) = default;            // Allow move
    Button& operator=(Button&&) = default;

    Button(const sf::Font& font, const std::string& text, const sf::Vector2f& position, 
           const sf::Vector2f& size, const sf::Color& bgColor, const sf::Color& textColor);
    
    void setTheme(const sf::Color& bgColor, const sf::Color& textColor, const sf::Color& borderColor);
    void setHighlightTheme(const ThemeConfig& themeConfig);
    void setDefaultTheme(const ThemeConfig& themeConfig);
    bool isHovered(const sf::Vector2i& mousePos) const;
    bool isClicked(const sf::Event& event, const sf::Vector2i& mousePos) const;
    bool isDoubleClicked(const sf::Event& event, const sf::Vector2i& mousePos);
    void resetDoubleClickFlag();
    void render(sf::RenderWindow& window);

private:
    sf::RectangleShape buttonShape;
    sf::Text buttonText;
    sf::Color backgroundColor;
    sf::Color textColor;
    sf::Color borderColor;
    
    sf::Clock doubleClickClock;
    bool doubleClickFlag;
};