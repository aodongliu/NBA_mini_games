#include "game/WhoHePlayFor.hpp"
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


WhoHePlayFor::WhoHePlayFor(sf::RenderWindow& window, std::shared_ptr<sf::Font> font)
    : GameBase(window, font), currentPlayerIndex(0) {
    resetGame();
}

void WhoHePlayFor::resetGame() {
    players.clear();
    currentPlayerIndex = 0;
    quitConfirmation = false;
    GameState = GameState::Running;
    
    setUpLabels();
    loadPlayers(size_t(NUM_PLAYER_TO_RANK));
    loadNextPlayer();
}

void WhoHePlayFor::setUpLabels() {
    const auto& theme = ThemeManager::getInstance().getThemeConfig();
    
    instructionLabel = Label(font, "Which team does this player play for?",
                           {windowSize.x * 0.05f, windowSize.y * 0.10f},
                           theme.instructionTextColor, 24);
                           
    errorLabel = Label(font, "",
                      {windowSize.x * 0.05f, windowSize.y * 0.325f},
                      theme.warningTextColor, 17);
}

void WhoHePlayFor::loadNextPlayer() {
    if (currentPlayerIndex >= players.size()) {
        instructionLabel.setText("Game Over! Press Enter to retry or ESC to return to the menu.");
        GameState = GameState::Ending;
        return;
    }

    const Player& player = players[currentPlayerIndex];
    if (!ResourceManager::getInstance().loadTexture(player.getHeadshotPath(), currentPlayerTexture)) {
        errorLabel.setText("Failed to load image for player: " + player.getFirstName() + " " + player.getLastName());
        errorClock.restart();
        return;
    }

    configurePlayerSprite();
}

void WhoHePlayFor::configurePlayerSprite() {
    currentPlayerSprite.setTexture(currentPlayerTexture);

    float leftWidth = windowSize.x * LEFT_SECTION_WIDTH_RATIO;
    float maxSpriteWidth = leftWidth * PLAYER_SPRITE_MAX_WIDTH_RATIO;
    float scale = maxSpriteWidth / currentPlayerTexture.getSize().x;

    currentPlayerSprite.setScale(scale, scale);

    float spriteX = (leftWidth - currentPlayerSprite.getGlobalBounds().width) / 2;
    float spriteY = (windowSize.y - currentPlayerSprite.getGlobalBounds().height) * 2 / 3;
    currentPlayerSprite.setPosition(spriteX, spriteY);
}

void WhoHePlayFor::handleEvent(const sf::Event& event) {
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

    //// Handle normal game events
    //if (event.type == sf::Event::TextEntered) {
    //    if (std::isdigit(event.text.unicode)) {
    //        userInput += static_cast<char>(event.text.unicode);
    //    } else if (event.text.unicode == '\b' && !userInput.empty()) {
    //        userInput.pop_back();
    //    }
    //} else if (event.type == sf::Event::KeyPressed) {
    //    if (event.key.code == sf::Keyboard::Enter) {
    //        int rank;
    //        std::string errorMsg;
    //        if (isValidInput(userInput, rank, errorMsg)) {
    //            if (rankings.count(rank)) {
    //                errorLabel.setText("Rank " + std::to_string(rank) + " is already occupied! Please choose another rank.");
    //                errorClock.restart();
    //            } else {
    //                rankings[rank] = std::make_shared<Player>(players[currentPlayerIndex]);
    //                currentPlayerIndex++;
    //                loadNextPlayer();
    //            }
    //            userInput.clear();
    //        } else {
    //            errorLabel.setText(errorMsg);
    //            errorClock.restart();
    //        }
    //    } else if (event.key.code == sf::Keyboard::Escape) {
    //        quitConfirmation = true;
    //        instructionLabel.setText("Are you sure you want to quit? Press Enter to confirm, ESC to cancel.");
    //    }
    //}
}

void WhoHePlayFor::render(sf::RenderWindow& window) {
    // Render Labels
    unsigned int textSize = (GameState == GameState::Ending or quitConfirmation) ? 30 : 24;
    sf::Uint32 textStyle = (GameState == GameState::Ending or quitConfirmation) ? sf::Text::Bold : sf::Text::Regular;
    instructionLabel.setSize(textSize);
    instructionLabel.setStyle(textStyle);
    instructionLabel.render(window);
    //inputLabel.setText("Your Input (press enter to confirm): " + userInput);
    //inputLabel.render(window);

    if (errorClock.getElapsedTime().asSeconds() < 3.0f && !errorLabel.getText().empty()) {
        errorLabel.render(window);
    }

    // Render player sprite and rankings
    window.draw(currentPlayerSprite);
}

void WhoHePlayFor::updateTheme() {
    GameBase::updateTheme();  // Call base class implementation
    // Add any WhoHePlayFor-specific theme updates here
}
