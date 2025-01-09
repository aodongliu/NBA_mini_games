#include "base/GameBase.hpp"
#include <iostream>
#include <fstream>
#include <random>

GameBase::GameBase(sf::RenderWindow& window, std::shared_ptr<sf::Font> font)
    : WindowBase(window)
    , font(font)
    , numPlayers(6)  // or whatever default you want
    , quitConfirmation(false)
    , GameState(GameState::Running)
{
    loadPlayers(numPlayers);
}

void GameBase::loadPlayers(size_t count) {
    try {
        DatabaseManager db;
        auto allPlayers = db.getAllPlayers();
        
        // Randomly shuffle players
        std::random_device rd;
        std::mt19937 gen(rd());
        std::shuffle(allPlayers.begin(), allPlayers.end(), gen);
        
        // Take only the number of players we need
        size_t playerCount = std::min(count, allPlayers.size());
        players.clear();
        
        for (size_t i = 0; i < playerCount; ++i) {
            players.emplace_back(allPlayers[i]);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error loading players: " << e.what() << std::endl;
    }
}

void GameBase::updateTheme() {
    const ThemeConfig& theme = ThemeManager::getInstance().getThemeConfig();
    instructionLabel.setColor(theme.instructionTextColor);
    errorLabel.setColor(theme.warningTextColor);
}
