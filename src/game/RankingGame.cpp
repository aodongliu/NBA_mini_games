#include "game/RankingGame.hpp"
#include <iostream>
#include <fstream>
#include <random>

RankingGame::RankingGame(const sf::Font& font, sf::Vector2u windowSize) 
    : currentPlayerIndex(0), errorClock(), quitConfirmation(false), font(font) {
    this->windowSize = windowSize;
    resetGame();
}

void RankingGame::resetGame() {
    rankings.clear();
    players.clear();
    currentPlayerIndex = 0;
    quitConfirmation = false;
    userInput.clear();
    subGameState = SubGameState::Running;
    
    instructionLabel = Label(font, "Rank this player (1-10). Press Enter to confirm, ESC to quit.",
                             {windowSize.x * 0.05f, windowSize.y * 0.10f},
                             ThemeManager::getInstance().getThemeConfig().instructionTextColor, 24, windowSize.x * 0.85f);
    errorLabel = Label(font, "", {windowSize.x * 0.05f, windowSize.y * 0.325f, },
                       ThemeManager::getInstance().getThemeConfig().warningTextColor, 17, windowSize.x * 0.5f);
    inputLabel = Label(font, "", {windowSize.x * 0.05f, windowSize.y * 0.25f},
                       ThemeManager::getInstance().getThemeConfig().highlightTextColor, 20, windowSize.x * 0.5f);
                           errorLabel.setText("");
    errorClock.restart();

    try {
        players = Player::loadPlayersFromCSV();
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(players.begin(), players.end(), g);
        players.resize(std::min(players.size(), size_t(10)));

        if (players.empty()) {
            throw std::runtime_error("Error: No players found in the CSV.");
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
    }

    loadNextPlayer();
}

void RankingGame::loadNextPlayer() {
    if (currentPlayerIndex >= players.size()) {
        instructionLabel.setText("Ranking complete! Press Enter to retry or ESC to return to the menu.");
        saveRankingToCSV();
        subGameState = SubGameState::Complete;
        return;
    }

    const Player& player = players[currentPlayerIndex];
    if (!player.loadImage(currentPlayerTexture)) {
        errorLabel.setText("Failed to load image for player: " + player.getFirstName() + " " + player.getLastName());
        errorClock.restart();
        return;
    }

    currentPlayerSprite.setTexture(currentPlayerTexture);

    float leftWidth = windowSize.x * 0.6f; // Left section width (60%)
    float maxSpriteWidth = leftWidth * 0.3f; // Max width is 30% of the left section
    float scale = maxSpriteWidth / currentPlayerTexture.getSize().x;
    currentPlayerSprite.setScale(scale, scale);

    float spriteX = (leftWidth - currentPlayerSprite.getGlobalBounds().width) / 2;
    float spriteY = (windowSize.y - currentPlayerSprite.getGlobalBounds().height) / 3 * 2;
    currentPlayerSprite.setPosition(spriteX, spriteY);
}

bool RankingGame::isValidInput(const std::string& input, int& rank, std::string& errorMsg) {
    if (input.empty()) {
        errorMsg = "Input cannot be empty! Please enter a number between 1 and 10.";
        return false;
    }

    for (char c : input) {
        if (!std::isdigit(c)) {
            errorMsg = "Invalid input! Please enter a valid number between 1 and 10.";
            return false;
        }
    }

    rank = std::stoi(input);

    if (rank < 1 || rank > 10) {
        errorMsg = "Invalid rank! Please enter a number between 1 and 10.";
        return false;
    }

    errorMsg.clear();
    return true;
}

void RankingGame::handleEvent(const sf::Event& event) {
    // Handle events when the game has ended
    if (subGameState == SubGameState::Complete) {
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Enter) {
                resetGame(); // Restart the game
                instructionLabel.setText("Rank this player (1-10). Press Enter to confirm, ESC to quit.");
            } else if (event.key.code == sf::Keyboard::Escape) {
                subGameState = SubGameState::Ended; // Exit to main menu
            }
        }
        return; // Exit early since no further handling is needed
    }

    // Handle quit confirmation logic
    if (quitConfirmation) {
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
            instructionLabel.setText("Exiting to the menu...");
            subGameState = SubGameState::Ended;
        } else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
            quitConfirmation = false;
            instructionLabel.setText("Rank this player (1-10). Press Enter to confirm, ESC to quit.");
        }
        return;
    }

    // Handle normal game events
    if (event.type == sf::Event::TextEntered) {
        if (std::isdigit(event.text.unicode)) {
            userInput += static_cast<char>(event.text.unicode);
        } else if (event.text.unicode == '\b' && !userInput.empty()) {
            userInput.pop_back();
        }
    } else if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Enter) {
            int rank;
            std::string errorMsg;
            if (isValidInput(userInput, rank, errorMsg)) {
                if (rankings.count(rank)) {
                    errorLabel.setText("Rank " + std::to_string(rank) + " is already occupied! Please choose another rank.");
                    errorClock.restart();
                } else {
                    rankings[rank] = std::make_shared<Player>(players[currentPlayerIndex]);
                    currentPlayerIndex++;
                    loadNextPlayer();
                }
                userInput.clear();
            } else {
                errorLabel.setText(errorMsg);
                errorClock.restart();
            }
        } else if (event.key.code == sf::Keyboard::Escape) {
            quitConfirmation = true;
            instructionLabel.setText("Are you sure you want to quit? Press Enter to confirm, ESC to cancel.");
        }
    }
}

void RankingGame::render(sf::RenderWindow& window) {
    // Render Labels
    unsigned int textSize = (subGameState == SubGameState::Complete or quitConfirmation) ? 30 : 24;
    sf::Uint32 textStyle = (subGameState == SubGameState::Complete or quitConfirmation) ? sf::Text::Bold : sf::Text::Regular;
    instructionLabel.setSize(textSize);
    instructionLabel.setStyle(textStyle);
    instructionLabel.render(window);
    inputLabel.setText("Your Input (press enter to confirm): " + userInput);
    inputLabel.render(window);

    if (errorClock.getElapsedTime().asSeconds() < 3.0f && !errorLabel.getText().empty()) {
        errorLabel.render(window);
    }

    // Render player sprite and rankings
    window.draw(currentPlayerSprite);
    displayRankings(window);
}

void RankingGame::displayRankings(sf::RenderWindow& window) {
    const ThemeConfig& themeConfig = ThemeManager::getInstance().getThemeConfig();

    float xStart = windowSize.x * 0.6f;              // Starting X position
    float xEnd = windowSize.x * 0.9f;                // Ending X position (90% of window width)
    float rightWidth = xEnd - xStart;                // Dynamically calculate width

    float yStart = windowSize.y * 0.3f;             // Starting Y position (30%)
    float yEnd = windowSize.y * 0.8f;                // Ending Y position (80%)
    float rankingsHeight = yEnd - yStart;            // Total height available for rankings

    // Background rectangle
    sf::RectangleShape background(sf::Vector2f(rightWidth, rankingsHeight));
    background.setFillColor(themeConfig.highlightAreaColor);
    background.setOutlineColor(themeConfig.highlightTextColor);
    background.setOutlineThickness(2);
    background.setPosition(xStart, yStart);
    window.draw(background);

    // Calculate row height dynamically based on available height
    float rowHeight = rankingsHeight / 10.0f; // Divide by 10 rows

    // Draw each ranking entry
    for (int i = 1; i <= 10; ++i) {
        std::string content = std::to_string(i) + ": ";
        if (rankings.count(i)) {
            content += rankings[i]->getFirstName() + " " + rankings[i]->getLastName();
        } else {
            content += "";
        }

        // Calculate position for each row
        float currentY = yStart + (i - 1) * rowHeight + rowHeight * 0.25f; // Add padding for text centering
        TextManager::drawText(window, content, font, 20, 
                              sf::Vector2f(xStart + 10, currentY), themeConfig.instructionTextColor);
    }
}


void RankingGame::saveRankingToCSV() {
    std::ofstream file("ranking.csv");
    if (!file) {
        std::cerr << "Failed to save rankings.\n";
        return;
    }

    file << "Rank,First Name,Last Name\n";
    for (const auto& [rank, playerPtr] : rankings) {
        file << rank << "," << playerPtr->getFirstName() << "," << playerPtr->getLastName() << "\n";
    }

    std::cout << "Rankings saved to rankings.csv.\n";
}
