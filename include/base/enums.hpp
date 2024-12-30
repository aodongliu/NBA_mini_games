#pragma once
#include <SFML/Graphics.hpp>

// Enum to represent the current state of the game
enum class GameState {
    Running,
    Ended,
    Ending,
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
    sf::Color highlightAreaColor;
    sf::Color warningTextColor;
    sf::Color buttonColor;
    sf::Color borderColor;
};