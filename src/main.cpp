#include <SFML/Graphics.hpp>
#include "base/enums.hpp"
#include "ui/GlobalUI.hpp"
#include "utils/ThemeManager.hpp"
#include "game/MainMenu.hpp"
#include "game/RankingGame.hpp"
#include <iostream>
#include "config.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "NBA Mini Games");

    sf::Font font;
    if (!font.loadFromFile(FONT_PATH "/Arial.ttf")) {
        std::cerr << "Error: Failed to load font.\n";
        return -1;
    }

    GlobalUI globalUI(font, window.getSize());
    MainMenu mainMenu(font, window.getSize());
    RankingGame rankingGame(font, window.getSize());
    GameState currentState = GameState::MainMenu;

    // Add menu options with callbacks
    mainMenu.addOption("Play Ranking Game", [&]() {
        std::cout << "Starting Ranking Game...\n";
        rankingGame.resetGame();
        currentState = GameState::RankingGame;
    });

    mainMenu.addOption("Quit", [&]() {
        std::cout << "Exiting Game...\n";
        window.close();
    });
    
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            globalUI.handleEvent(event, window);

            switch (currentState) {
                case GameState::MainMenu:
                    mainMenu.handleEvent(event, window);
                    break;

                case GameState::RankingGame:
                    std::cout << "Current State: RankingGame\n";
                    rankingGame.handleEvent(event);

                    if (rankingGame.subGameState == SubGameState::Ended) {
                        std::cout << "Ranking Game Ended. Returning to MainMenu...\n";
                        currentState = GameState::MainMenu; // Transition back to MainMenu
                        mainMenu.reset();
                    }
                    break;
            }
        }

        // Rendering based on the current state
        window.clear(ThemeManager::getInstance().getThemeConfig().backgroundColor);
        globalUI.render(window);
        if (currentState == GameState::MainMenu) {
            mainMenu.render(window);
        } else if (currentState == GameState::RankingGame) {
            rankingGame.render(window);
        }
        window.display();
    }

    return 0;
}
