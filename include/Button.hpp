#pragma once
#include <SFML/Graphics.hpp>
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
    bool isHovered(const sf::Vector2i& mousePos) const;
    bool isClicked(const sf::Event& event, const sf::Vector2i& mousePos) const;
    bool updateHoverState(const sf::Vector2i& mousePos, const sf::Color& hoverBgColor, const sf::Color& hoverTextColor);
    void render(sf::RenderWindow& window);

private:
    bool isHoveredState;
    sf::RectangleShape buttonShape;
    sf::Text buttonText;
    sf::Color backgroundColor;
    sf::Color textColor;
    sf::Color borderColor;

    sf::Color defaultBgColor;
    sf::Color defaultTextColor;
};