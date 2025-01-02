#include "base/GameBase.hpp"
#include <iostream>
#include <fstream>
#include <random>

GameBase::GameBase(sf::RenderWindow& window, std::shared_ptr<sf::Font> font)
    : WindowBase(window), font(font), errorClock(), quitConfirmation(false) {
}

void GameBase::loadRandomPlayers(size_t numPlayers) {
    try {
        players = Player::loadPlayersFromCSV();
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(players.begin(), players.end(), g);
        players.resize(std::min(players.size(), numPlayers));
        if (players.empty()) {
            throw std::runtime_error("Error: No players found in the CSV.");
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
    }
}

void GameBase::updateTheme() {
    const ThemeConfig& theme = ThemeManager::getInstance().getThemeConfig();
    instructionLabel.setColor(theme.instructionTextColor);
    errorLabel.setColor(theme.warningTextColor);
}
