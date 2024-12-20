#pragma once

#include "base/UIComponent.hpp"
#include "utils/textUtils.hpp"
#include <SFML/Graphics.hpp>

class Label : public UIComponent {
public:
    Label();
    Label(const sf::Font& font, const std::string& text, sf::Vector2f position,
          sf::Color color, unsigned int size, float maxWidth = 0.0f, sf::Uint32 style = sf::Text::Regular);

    void setText(const std::string& text);
    void setColor(sf::Color color);
    void setFont(const sf::Font& font);
    void setSize(unsigned int size);
    void setStyle(sf::Uint32 style);
    void setMaxWidth(float maxWidth);

    std::string getText() const;

    void render(sf::RenderWindow& window) override;

private:
    sf::Text labelText;
    std::string rawText;
    float maxWidth; 
};
