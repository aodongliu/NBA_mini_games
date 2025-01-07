#include "utils/textUtils.hpp"

namespace TextUtils {
    std::string wrapText(const std::string& content, const sf::Font& font, unsigned int size, float maxWidth) {
        if (content.empty() || maxWidth <= 0) {
            return content;
        }

        std::string result;
        std::string currentLine;
        std::string word;
        
        sf::Text measurer;
        measurer.setFont(font);
        measurer.setCharacterSize(size);

        for (size_t i = 0; i < content.length(); ++i) {
            if (content[i] == ' ' || content[i] == '\n' || i == content.length() - 1) {
                // Include the last character if we're at the end
                if (i == content.length() - 1 && content[i] != ' ' && content[i] != '\n') {
                    word += content[i];
                }

                // Measure current line + new word
                std::string testLine = currentLine + (currentLine.empty() ? "" : " ") + word;
                measurer.setString(testLine);
                
                if (measurer.getLocalBounds().width > maxWidth && !currentLine.empty()) {
                    // Add current line to result and start new line with word
                    result += currentLine + "\n";
                    currentLine = word;
                } else {
                    // Add word to current line
                    if (!currentLine.empty()) {
                        currentLine += " ";
                    }
                    currentLine += word;
                }

                if (content[i] == '\n') {
                    result += currentLine + "\n";
                    currentLine.clear();
                }

                word.clear();
            } else {
                word += content[i];
            }
        }

        // Add the last line if there is one
        if (!currentLine.empty()) {
            result += currentLine;
        }

        return result;
    }
}