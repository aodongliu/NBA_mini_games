#include <SFML/Graphics.hpp>
#include "MainMenu.hpp"
#include "RankingGame.hpp"
#include <iostream>
#include "config.hpp"
#include "enums.hpp"
#include "ThemeManager.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "NBA Mini Games");

    sf::Font font;
    if (!font.loadFromFile(FONT_PATH "/Arial.ttf")) {
        std::cerr << "Error: Failed to load font.\n";
        return -1;
    }

    ThemeManager& themeManager = ThemeManager::getInstance();

    MainMenu mainMenu(font, window.getSize());
    RankingGame rankingGame(font, window.getSize());
    GameState currentState = GameState::MainMenu;
    
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            themeManager.handleThemeToggle(event, sf::Mouse::getPosition(window));

            switch (currentState) {
                case GameState::MainMenu:
                    mainMenu.handleEvent(event, window);
                    if (auto action = mainMenu.handleAction()) {
                        switch (*action) {
                            case 0:
                                std::cout << "Starting Ranking Game...\n";
                                rankingGame.resetGame();
                                rankingGame.subGameState = SubGameState::Running;
                                currentState = GameState::RankingGame;
                                break;

                            case 1:
                                std::cout << "Exiting Game...\n";
                                window.close();
                                break;
                        }
                    }
                    break;

                case GameState::RankingGame:
                    rankingGame.handleEvent(event);

                    if (rankingGame.subGameState == SubGameState::Ended) {
                        currentState = GameState::MainMenu; // Transition back to MainMenu
                    }
                    break;
            }
        }

        // Rendering based on the current state
        window.clear(themeManager.getThemeConfig().backgroundColor);
        if (currentState == GameState::MainMenu) {
            mainMenu.render(window);
        } else if (currentState == GameState::RankingGame) {
            rankingGame.render(window);
        }
        themeManager.renderThemeButton(window);
        window.display();
    }

    return 0;
}
