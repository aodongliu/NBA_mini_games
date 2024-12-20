#pragma once

#include <SFML/Graphics.hpp>

class UIComponent {
public:
    UIComponent();
    virtual ~UIComponent() = default;

    virtual void render(sf::RenderWindow& window) = 0;
    virtual bool isHovered(const sf::Vector2i& mousePos) const;

    void setPosition(const sf::Vector2f& position);
    void setSize(const sf::Vector2f& size);
    sf::Vector2f getPosition() const;
    sf::Vector2f getSize() const;

    void setVisible(bool visible);
    bool isVisible() const;

protected:
    sf::Vector2f position;
    sf::Vector2f size;
    bool visible;

};