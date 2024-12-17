#pragma once
#include <SFML/Graphics.hpp>

// Enum to represent the current state of the game
enum class GameState {
    MainMenu,    // Main menu screen
    RankingGame, // Ranking game screen
    // Future games can be added here
};

enum class SubGameState{
    Running,
    Ended,
};

enum class Theme {
    Light,
    Dark,
};

struct ThemeConfig
{
    sf::Color backgroundColor;
    sf::Color instructionTextColor;
    sf::Color highlightTextColor;
    sf::Color warningTextColor;
    sf::Color buttonColor;
    sf::Color borderColor;
};