#include "TextManager.hpp"
#include <sstream>

void TextManager::drawText(sf::RenderWindow& window, const std::string& content, 
                           const sf::Font& font, unsigned int size, sf::Vector2f position,
                           sf::Color color, float maxWidth) {

    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(size);
    text.setFillColor(color);
    text.setPosition(position);
    
    std::string wrappedContent = content;
    if (maxWidth > 0.0f) {
        wrappedContent = wrapText(content, font, size, maxWidth);
    }
    
    text.setString(wrappedContent);
    window.draw(text);

}

std::string TextManager::wrapText(const std::string& content, const sf::Font& font,
                                  unsigned int size, float maxWidth) {

    std::string wrapped;
    std::string currentLine;
    std::istringstream stream(content);
    std::string word;

    sf::Text tempText;
    tempText.setFont(font);
    tempText.setCharacterSize(size);

        while (stream >> word) {
        std::string testLine = currentLine.empty() ? word : currentLine + " " + word;
        tempText.setString(testLine);

        if (tempText.getLocalBounds().width > maxWidth) {
            wrapped += currentLine + "\n"; // Add current line to the result
            currentLine = word;            // Start a new line with the word
        } else {
            currentLine = testLine;        // Append word to current line
        }
    }

    wrapped += currentLine; // Add the last line
    return wrapped;

}

float TextManager::getXCenter(const std::string& content, const sf::Font& font, unsigned int size, float windowWidth) {
    sf::Text tempText;
    tempText.setFont(font);
    tempText.setString(content);
    tempText.setCharacterSize(size);

    float textWidth = tempText.getLocalBounds().width;
    return (windowWidth - textWidth) / 2.0f;
}