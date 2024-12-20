#include "base/UIComponent.hpp"

UIComponent::UIComponent() : position(0, 0), visible(true) {}

// Setters
void UIComponent::setPosition(const sf::Vector2f& position) {
    this->position = position;
}
void UIComponent::setSize(const sf::Vector2f& size) {
    this->size = size;
}
void UIComponent::setVisible(bool visible) {
    this->visible = visible;
}

// Getters
sf::Vector2f UIComponent::getPosition() const {
    return position;
}
sf::Vector2f UIComponent::getSize() const {
    return size;
}
bool UIComponent::isVisible() const {
    return visible;
}

bool UIComponent::isHovered(const sf::Vector2i& mousePos) const {
    sf::FloatRect bounds(position, size);
    return bounds.contains(static_cast<sf::Vector2f>(mousePos));
}
