#include "ui/Label.hpp"
#include "utils/textUtils.hpp"

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
    setText(text);
}

void Label::setText(const std::string& text) {
    rawText = text;
    updateText();
}

void Label::setColor(sf::Color color) {
    labelText.setFillColor(color);
}

void Label::setFont(std::shared_ptr<sf::Font> font) {
    this->font = font;
    labelText.setFont(*font);
    updateText();
}

void Label::setSize(unsigned int size) {
    labelText.setCharacterSize(size);
    updateText();
}

void Label::setStyle(sf::Uint32 style) {
    labelText.setStyle(style);
    updateText();
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

void Label::updateText() {
    if (maxWidth > 0) {
        std::string wrappedText = TextUtils::wrapText(rawText, *font, labelText.getCharacterSize(), maxWidth);
        labelText.setString(wrappedText);
    } else {
        labelText.setString(rawText);
    }
}
