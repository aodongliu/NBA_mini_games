#ifndef UI_MANAGER_HPP
#define UI_MANAGER_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include <map>

class UIManager {
public:
    UIManager(const std::string& fontPath);

    void render(sf::RenderWindow& window, const sf::Sprite& playerSprite, const std::string& userInput,
                const std::map<int, sf::Sprite>& rankingSprites, const std::string& errorMsg,
                const std::map<int, std::string>& rankingNames); // Added rankingNames parameter

    sf::Text createText(const std::string& content, unsigned int size, sf::Vector2f position);

private:
    sf::Font font;
    sf::Text rankingText;
    sf::Text errorText; // To display error messages
    sf::Text rankLabels[10]; // Labels for 1–10
};

#endif // UI_MANAGER_HPP

