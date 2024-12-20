#include "ui/Label.hpp"
Label::Label() : maxWidth(0.0f) {}

Label::Label(const sf::Font& font, const std::string& text, sf::Vector2f position, sf::Color color, unsigned int size, 
             float maxWidth, sf::Uint32 style)
    : maxWidth(maxWidth), rawText(text) {
    labelText.setFont(font);
    labelText.setString(TextUtils::wrapText(text, font, size, maxWidth));
    labelText.setPosition(position);
    labelText.setFillColor(color);
    labelText.setCharacterSize(size);
    labelText.setStyle(style);
}

void Label::setText(const std::string& text) {
    rawText = text;
    std::string wrappedString = TextUtils::wrapText(text, *labelText.getFont(), labelText.getCharacterSize(), maxWidth);
    labelText.setString(wrappedString);
}

void Label::setColor(sf::Color color) {
    labelText.setFillColor(color);
}

void Label::setFont(const sf::Font& font) {
    labelText.setFont(font);
    setText(rawText);
}

void Label::setSize(unsigned int size) {
    labelText.setCharacterSize(size);
    setText(rawText);
}

void Label::setStyle(sf::Uint32 style) {
    labelText.setStyle(style);
    setText(rawText);
}

void Label::setMaxWidth(float maxWidth) {
    this->maxWidth = maxWidth;
    setText(rawText);
}

std::string Label::getText() const {
    return labelText.getString();
}

void Label::render(sf::RenderWindow& window) {
    if (!isVisible()) return; // Don't render if not visible
    window.draw(labelText);
}
