#pragma once

#include <string>
#include <SFML/Graphics.hpp>

namespace TextUtils {
    // Wraps text to fit within a maximum width
    std::string wrapText(const std::string& content, const sf::Font& font, unsigned int size, float maxWidth);
}