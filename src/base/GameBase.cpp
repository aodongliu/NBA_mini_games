#include "base/GameBase.hpp"
#include "game/Player.hpp"
#include <iostream>

GameBase::GameBase(sf::RenderWindow& window, std::shared_ptr<sf::Font> font)
    : WindowBase(window)
    , font(font)
    , numPlayers(6)
    , quitConfirmation(false)
    , GameState(GameState::Running)
{
    loadPlayers(numPlayers);
}

void GameBase::loadPlayers(size_t count) {
    try {
        auto allPlayers = Player::getPlayersAbovePoints(0.0f);
        if (allPlayers.empty()) {
            std::cerr << "No players loaded from database\n";
            return;
        }

        size_t actualCount = std::min(count, allPlayers.size());
        players.assign(allPlayers.begin(), allPlayers.begin() + actualCount);

    } catch (const std::exception& e) {
        std::cerr << "Error loading players: " << e.what() << std::endl;
    }
}

void GameBase::updateTheme() {
    const ThemeConfig& theme = ThemeManager::getInstance().getThemeConfig();
    instructionLabel.setColor(theme.instructionTextColor);
    errorLabel.setColor(theme.warningTextColor);
}
