#include "game/RankingGame.hpp"
#include <iostream>
#include <fstream>
#include <random>

// Left section: Display player pictures
constexpr float LEFT_SECTION_WIDTH_RATIO = 0.6f;
constexpr float PLAYER_SPRITE_MAX_WIDTH_RATIO = 0.3f;
// Right section: Display a live ranking tale
constexpr float RIGHT_SECTION_WIDTH_RATIO = 0.3f;
constexpr float RANKING_TABLE_YSTART_RATIO = 0.3f;
constexpr float RANKING_TABLE_HEIGHT_RATIO = 0.5f;
constexpr float LABEL_PADDING_RATIO = 0.25f;

constexpr float ERROR_MESSAGE_DISPLAY_TIME = 3.0f;
constexpr size_t NUM_PLAYER_TO_RANK = 6;

RankingGame::RankingGame(sf::RenderWindow& window, std::shared_ptr<sf::Font> font)
    : GameBase(window, font), currentPlayerIndex(0) {
    resetGame();
}

void RankingGame::resetGame() {
    rankings.clear();
    players.clear();
    currentPlayerIndex = 0;
    quitConfirmation = false;
    userInput.clear();
    GameState = GameState::Running;
    
    setUpLabels();
    loadRandomPlayers(size_t(NUM_PLAYER_TO_RANK));
    loadNextPlayer();
}

void RankingGame::setUpLabels() {
    const auto& theme = ThemeManager::getInstance().getThemeConfig();

    instructionLabel = Label(font, "Rank this player (1-10). Press Enter to confirm, ESC to quit.",
                             {windowSize.x * 0.05f, windowSize.y * 0.10f},
                             theme.instructionTextColor, 24, windowSize.x * 0.85f);
    errorLabel = Label(font, "", {windowSize.x * 0.05f, windowSize.y * 0.325f},
                       theme.warningTextColor, 17, windowSize.x * 0.5f);
    inputLabel = Label(font, "", {windowSize.x * 0.05f, windowSize.y * 0.25f},
                       theme.highlightTextColor, 20, windowSize.x * 0.5f);

    errorClock.restart();
}

void RankingGame::loadNextPlayer() {
    if (currentPlayerIndex >= players.size()) {
        instructionLabel.setText("Ranking complete! Press Enter to retry or ESC to return to the menu.");
        saveRankingToCSV();
        GameState = GameState::Ending;
        return;
    }

    const Player& player = players[currentPlayerIndex];
    if (!player.loadImage(currentPlayerTexture)) {
        errorLabel.setText("Failed to load image for player: " + player.getFirstName() + " " + player.getLastName());
        errorClock.restart();
        return;
    }

    configurePlayerSprite();
}

void RankingGame::configurePlayerSprite() {
    currentPlayerSprite.setTexture(currentPlayerTexture);

    float leftWidth = windowSize.x * LEFT_SECTION_WIDTH_RATIO;
    float maxSpriteWidth = leftWidth * PLAYER_SPRITE_MAX_WIDTH_RATIO;
    float scale = maxSpriteWidth / currentPlayerTexture.getSize().x;

    currentPlayerSprite.setScale(scale, scale);

    float spriteX = (leftWidth - currentPlayerSprite.getGlobalBounds().width) / 2;
    float spriteY = (windowSize.y - currentPlayerSprite.getGlobalBounds().height) * 2 / 3;
    currentPlayerSprite.setPosition(spriteX, spriteY);
}

bool RankingGame::isValidInput(const std::string& input, int& rank, std::string& errorMsg) {
    if (input.empty()) {
        errorMsg = "Input cannot be empty! Please enter a number between 1 to " + std::to_string(NUM_PLAYER_TO_RANK) + ".";
        return false;
    }

    if (!std::all_of(input.begin(), input.end(), ::isdigit)) {
        errorMsg = "Invalid input! Please enter a valid number between 1 to " + std::to_string(NUM_PLAYER_TO_RANK) + ".";
        return false;
    }

    rank = std::stoi(input);
    if (rank < 1 || rank > NUM_PLAYER_TO_RANK) {
        errorMsg = "Invalid rank! Please enter a number between 1 to " + std::to_string(NUM_PLAYER_TO_RANK) + ".";
        return false;
    }

    errorMsg.clear();
    return true;
}

void RankingGame::handleEvent(const sf::Event& event) {
    // Handle events when the game has ended
    if (GameState == GameState::Ending) {
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Enter) {
                resetGame(); // Restart the game
            } else if (event.key.code == sf::Keyboard::Escape) {
                GameState = GameState::Ended; // Exit to main menu
            }
        }
        return; // Exit early since no further handling is needed
    }

    // Handle quit confirmation logic
    if (quitConfirmation) {
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
            instructionLabel.setText("Exiting to the menu...");
            GameState = GameState::Ended;
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
    unsigned int textSize = (GameState == GameState::Ending or quitConfirmation) ? 30 : 24;
    sf::Uint32 textStyle = (GameState == GameState::Ending or quitConfirmation) ? sf::Text::Bold : sf::Text::Regular;
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

    float xStart = windowSize.x * LEFT_SECTION_WIDTH_RATIO;
    float rightWidth = windowSize.x * RIGHT_SECTION_WIDTH_RATIO;
    float yStart = windowSize.y * RANKING_TABLE_YSTART_RATIO;
    float rankingsHeight = windowSize.y * RANKING_TABLE_HEIGHT_RATIO;

    sf::RectangleShape background(sf::Vector2f(rightWidth, rankingsHeight));
    background.setFillColor(themeConfig.highlightAreaColor);
    background.setOutlineColor(themeConfig.highlightTextColor);
    background.setOutlineThickness(2);
    background.setPosition(xStart, yStart);
    window.draw(background);

    float rowHeight = rankingsHeight / NUM_PLAYER_TO_RANK;

    for (int i = 1; i <= NUM_PLAYER_TO_RANK; ++i) {
        std::string content = std::to_string(i) + ": ";
        if (rankings.count(i)) {
            content += rankings[i]->getFirstName() + " " + rankings[i]->getLastName();
        }
        float currentY = yStart + (i - 1) * rowHeight + rowHeight * LABEL_PADDING_RATIO;
        //TextManager::drawText(window, content, font, 20, {xStart + 10, currentY}, themeConfig.instructionTextColor);
        Label rankLabel(font, content, {xStart + 10, currentY}, themeConfig.instructionTextColor, 20);
        rankLabel.render(window);
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

void RankingGame::updateTheme() {
    GameBase::updateTheme();
    const ThemeConfig& theme = ThemeManager::getInstance().getThemeConfig();
    inputLabel.setColor(theme.highlightTextColor);
    inputLabel.setColor(theme.highlightTextColor);
}
