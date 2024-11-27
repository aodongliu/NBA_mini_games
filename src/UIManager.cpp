#include "UIManager.hpp"
#include <iostream>


UIManager::UIManager(const std::string& fontPath) {
    if (!font.loadFromFile(fontPath)) {
        throw std::runtime_error("Error: Failed to load font " + fontPath);
    }

    // Initialize ranking labels (1–10)
    for (int i = 0; i < 10; ++i) {
        rankLabels[i].setFont(font);
        rankLabels[i].setString(std::to_string(i + 1) + ":");
        rankLabels[i].setCharacterSize(20);
        rankLabels[i].setFillColor(sf::Color::White);
        rankLabels[i].setPosition(700, 50 + i * 60); // Align to the right
    }

    // Initialize error text
    errorText.setFont(font);
    errorText.setCharacterSize(18);
    errorText.setFillColor(sf::Color::Red);
    errorText.setPosition(20, 60);
}

sf::Text UIManager::createText(const std::string& content, unsigned int size, sf::Vector2f position) {
    sf::Text text;
    text.setFont(font);
    text.setString(content);
    text.setCharacterSize(size);
    text.setFillColor(sf::Color::White);
    text.setPosition(position);
    return text;
}

void UIManager::render(sf::RenderWindow& window, const sf::Sprite& playerSprite, const std::string& userInput,
                       const std::map<int, sf::Sprite>& rankingSprites, const std::string& errorMsg,
                       const std::map<int, std::string>& rankingNames) {
    window.clear(sf::Color::Black);

    // Get window dimensions
    sf::Vector2u windowSize = window.getSize();
    float leftWidth = windowSize.x * 0.6f;  // Left section width (60% of total width)
    float rightWidth = windowSize.x * 0.4f; // Right section width
    float totalHeight = windowSize.y;

    // Save original dimensions of the sprite for RHS scaling
    float originalSpriteWidth = playerSprite.getGlobalBounds().width;
    float originalSpriteHeight = playerSprite.getGlobalBounds().height;

    // Scale and position the main player sprite (LHS)
    sf::Sprite centeredSprite = playerSprite;
    float maxSpriteWidth = leftWidth * 0.35f; // Max width is 35% of the LHS width
    float scale = maxSpriteWidth / originalSpriteWidth;
    centeredSprite.setScale(scale, scale);

    // Position the sprite in the center of the LHS
    float spriteX = (leftWidth - centeredSprite.getGlobalBounds().width) / 2;
    float spriteY = (totalHeight - centeredSprite.getGlobalBounds().height) / 2;
    centeredSprite.setPosition(spriteX, spriteY);

    // Create a perfectly fitting border rectangle
    sf::RectangleShape backgroundRect;
    backgroundRect.setSize(sf::Vector2f(centeredSprite.getGlobalBounds().width + 20,
                                        centeredSprite.getGlobalBounds().height + 20));
    backgroundRect.setFillColor(sf::Color(50, 50, 50)); // Dark gray background
    backgroundRect.setOutlineColor(sf::Color::White);   // White border
    backgroundRect.setOutlineThickness(5);
    backgroundRect.setPosition(spriteX - 10, spriteY - 10);

    // Draw the background rectangle and the sprite
    window.draw(backgroundRect);
    window.draw(centeredSprite);

    // Draw ranking text above the sprite
    rankingText.setString("Enter rank (1-10): " + userInput);
    rankingText.setFont(font);
    rankingText.setCharacterSize(24);
    rankingText.setFillColor(sf::Color::White);
    rankingText.setPosition(20, 20);
    window.draw(rankingText);

    // Draw error message if any
    if (!errorMsg.empty()) {
        errorText.setString(errorMsg);
        window.draw(errorText);
    }

    // Live list (RHS): Two columns (5 each)
    float startX1 = leftWidth + 20;              // Start of the first column in the right section
    float startX2 = leftWidth + (rightWidth / 2); // Start of the second column
    float topMargin = totalHeight / 7;           // Leave 1/7 blank at the top
    float rowHeight = totalHeight / 7;           // Total space allocated per row
    float pictureHeight = rowHeight * 0.7f;      // Height of the picture in each row
    float nameYOffset = rowHeight * 0.15f;       // Offset to align the name with the number

    for (int i = 1; i <= 10; ++i) {
        // Determine column and row
        bool isLeftColumn = (i <= 5);
        float currentX = isLeftColumn ? startX1 : startX2;
        float currentY = topMargin + ((isLeftColumn ? i - 1 : i - 6) * rowHeight);

        // Draw rank label
        sf::Text rankLabel = createText(std::to_string(i) + ":", 20, sf::Vector2f(currentX, currentY + nameYOffset));
        window.draw(rankLabel);

        // Draw player name aligned with the number
        if (rankingNames.find(i) != rankingNames.end()) {
            sf::Text playerName = createText(rankingNames.at(i), 18, sf::Vector2f(currentX + 50, currentY + nameYOffset));
            window.draw(playerName);
        }

        // Draw player sprite directly below the name
        if (rankingSprites.find(i) != rankingSprites.end()) {
            sf::Sprite sprite = rankingSprites.at(i);

            // Calculate scaling factors based on the saved original height
            float heightScale = pictureHeight / originalSpriteHeight;

            // Apply the scaling factor
            sprite.setScale(heightScale, heightScale);

            // Position the sprite
            sprite.setPosition(currentX + 50, currentY + nameYOffset + 30); // Offset for alignment below the name
            window.draw(sprite);
        }
    }

    window.display();
}

