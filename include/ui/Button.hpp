#pragma once
#include <SFML/Graphics.hpp>
#include "base/UIComponent.hpp"
#include "base/enums.hpp"
#include <string>

class Button : public UIComponent {
public:
    Button();
    Button(std::shared_ptr<sf::Font> font, const std::string& text, const sf::Vector2f& position, 
           const sf::Vector2f& size, const sf::Color& bgColor, const sf::Color& textColor);
    
    void setCallback(std::function<void()> onClickCallback);
    void setTheme(const sf::Color& bgColor, const sf::Color& textColor, const sf::Color& borderColor);
    void setHighlightTheme(const ThemeConfig& themeConfig);
    void setDefaultTheme(const ThemeConfig& themeConfig);

    bool isHovered(const sf::Vector2i& mousePos) const override;
    bool isClicked(const sf::Event& event, const sf::Vector2i& mousePos) const;
    bool isDoubleClicked(const sf::Event& event, const sf::Vector2i& mousePos);
    
    void triggerCallback();
    void render(sf::RenderWindow& window) override;

    void setIcon(const sf::Texture& texture);

private:
    sf::RectangleShape buttonShape;
    sf::Text buttonText;

    std::function<void()> onClick;
    sf::Clock doubleClickClock;

    sf::Sprite icon;
    bool hasIcon = false;

    void centerText();
};