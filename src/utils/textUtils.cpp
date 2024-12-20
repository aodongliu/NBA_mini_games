#include "utils/textUtils.hpp"
#include <sstream>

namespace TextUtils {

    std::string wrapText(const std::string& content, const sf::Font& font, unsigned int size, float maxWidth) {
        if (maxWidth <= 0.0f) return content;

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
                wrapped += currentLine + "\n";
                currentLine = word;
            } else {
                currentLine = testLine;
            }
        }

        if (!currentLine.empty()) {
            wrapped += currentLine;
        }

        return wrapped;
    }

}