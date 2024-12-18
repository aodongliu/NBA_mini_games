#pragma once

#include <SFML/Graphics.hpp>
#include <string>

class TextManager {
public:
    static void drawText(sf::RenderWindow& window, const std::string& content,
                         const sf::Font& font, unsigned int size, sf::Vector2f position,
                         sf::Color color, float maxWidth = 0.0f, sf::Uint32 style = sf::Text::Regular);

    static float getXCenter(const std::string& content, const sf::Font& font, unsigned int size, float windowWidth);

private:
    static std::string wrapText(const std::string& content, const sf::Font& font,
                                unsigned int size, float maxWidth);
};