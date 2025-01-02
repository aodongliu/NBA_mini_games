#include "ui/Label.hpp"

Label::Label() : maxWidth(0.0f) {
    labelText.setString("");
}

Label::Label(std::shared_ptr<sf::Font> font, const std::string& text, sf::Vector2f position,
             sf::Color color, unsigned int size, float maxWidth, sf::Uint32 style)
    : font(font), maxWidth(maxWidth) {
    labelText.setFont(*font);
    labelText.setString(text);
    labelText.setPosition(position);
    labelText.setFillColor(color);
    labelText.setCharacterSize(size);
    labelText.setStyle(style);
    rawText = text;
}

void Label::setText(const std::string& text) {
    rawText = text;
    labelText.setString(text);
}

void Label::setColor(sf::Color color) {
    labelText.setFillColor(color);
}

void Label::setFont(std::shared_ptr<sf::Font> font) {
    this->font = font;
    labelText.setFont(*font);
}

void Label::setSize(unsigned int size) {
    labelText.setCharacterSize(size);
}

void Label::setStyle(sf::Uint32 style) {
    labelText.setStyle(style);
}

void Label::setMaxWidth(float maxWidth) {
    this->maxWidth = maxWidth;
}

std::string Label::getText() const {
    return rawText;
}

void Label::render(sf::RenderWindow& window) {
    window.draw(labelText);
}
