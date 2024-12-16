#pragma once

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