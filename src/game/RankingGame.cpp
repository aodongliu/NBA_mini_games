#include "game/RankingGame.hpp"
#include <iostream>
#include <fstream>
#include <random>

RankingGame::RankingGame(const sf::Font& font, sf::Vector2u windowSize) 
    : currentPlayerIndex(0), errorClock(), quitConfirmation(false), font(font) {
    this->windowSize = windowSize;
    resetGame();
}

void RankingGame::setInstruction(const std::string& message) {
    instructionMessage = message;
}

void RankingGame::setError(const std::string& message) {
    errorMessage = message;
    errorClock.restart();
}

void RankingGame::resetGame() {
    rankings.clear();
    players.clear();
    currentPlayerIndex = 0;
    quitConfirmation = false;
    userInput.clear();
    errorMessage.clear();
    subGameState = SubGameState::Running;

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

    setInstruction("Rank this player (1-10). Press Enter to confirm, ESC to quit.");
    loadNextPlayer();
}

void RankingGame::loadNextPlayer() {
    if (currentPlayerIndex >= players.size()) {
        setInstruction("Ranking complete! Press Enter to retry or ESC to return to the menu.");
        saveRankingToCSV();
        subGameState = SubGameState::Complete;
        return;
    }

    const Player& player = players[currentPlayerIndex];
    if (!player.loadImage(currentPlayerTexture)) {
        setError("Failed to load image for player: " + player.getFirstName() + " " + player.getLastName());
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
                setInstruction("Rank this player (1-10). Press Enter to confirm, ESC to quit.");
            } else if (event.key.code == sf::Keyboard::Escape) {
                subGameState = SubGameState::Ended; // Exit to main menu
            }
        }
        return; // Exit early since no further handling is needed
    }

    // Existing logic for handling normal game events
    if (quitConfirmation) {
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
            setInstruction("Exiting to the menu...");
            subGameState = SubGameState::Ended;
        } else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
            quitConfirmation = false;
            setInstruction("Rank this player (1-10). Press Enter to confirm, ESC to quit.");
        }
        return;
    }

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
                    setError("Rank " + std::to_string(rank) + " is already occupied! Please choose another rank.");
                } else {
                    rankings[rank] = std::make_shared<Player>(players[currentPlayerIndex]);
                    currentPlayerIndex++;
                    loadNextPlayer();
                }
                userInput.clear();
            } else {
                setError(errorMsg);
            }
        } else if (event.key.code == sf::Keyboard::Escape) {
            quitConfirmation = true;
            setInstruction("Are you sure you want to quit? Press Enter to confirm, ESC to cancel.");
        }
    }
}

void RankingGame::render(sf::RenderWindow& window) {
    
    const ThemeConfig& themeConfig = ThemeManager::getInstance().getThemeConfig();
    
    unsigned int textSize = (subGameState == SubGameState::Complete or quitConfirmation) ? 30 : 24;
    sf::Uint32 textStyle = (subGameState == SubGameState::Complete or quitConfirmation) ? sf::Text::Bold : sf::Text::Regular;

    TextManager::drawText(window, instructionMessage, font, textSize,
                          sf::Vector2f(windowSize.x * 0.05f, windowSize.y * 0.10f), 
                          themeConfig.instructionTextColor, windowSize.x * 0.85f, textStyle);

    TextManager::drawText(window, "Your Input (press enter to confirm): " + userInput,
                          font, 20, sf::Vector2f(windowSize.x * 0.05f, windowSize.y * 0.25f),
                          themeConfig.highlightTextColor, windowSize.x * 0.5f);

    if (errorClock.getElapsedTime().asSeconds() < 3.0f && !errorMessage.empty()) {
        TextManager::drawText(window, errorMessage, font, 17, 
                              sf::Vector2f(windowSize.x * 0.05f, windowSize.y * 0.325f), 
                              themeConfig.warningTextColor, windowSize.x * 0.5f);
    }

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
