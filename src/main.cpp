#include <SFML/Graphics.hpp>
#include "MainMenu.hpp"
#include "RankingGame.hpp"
#include <iostream>
#include "config.hpp"
#include "enums.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "NBA Mini Games");

    sf::Font font;
    if (!font.loadFromFile(FONT_PATH "/Arial.ttf")) {
        std::cerr << "Error: Failed to load font.\n";
        return -1;
    }

    MainMenu mainMenu(font, window.getSize());
    RankingGame rankingGame(font, window.getSize());
    GameState currentState = GameState::MainMenu;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            switch (currentState) {
                case GameState::MainMenu:
                    mainMenu.handleEvent(event);

                    if (event.type == sf::Event::KeyPressed) {
                        if (event.key.code == sf::Keyboard::Enter) {
                            int choice = mainMenu.getSelectedOption();
                            if (choice == 0) {
                                std::cout << "Starting Ranking Game...\n";
                                currentState = GameState::RankingGame; // Transition to RankingGame
                            } else if (choice == 1) {
                                std::cout << "Exiting Game...\n";
                                window.close();
                            }
                        }
                    }

                    if (mainMenu.isOptionDoubleClicked()) {
                        int choice = mainMenu.getSelectedOption();
                        if (choice == 0) {
                            std::cout << "Starting Ranking Game...\n";
                            currentState = GameState::RankingGame; // Transition to RankingGame
                        } else if (choice == 1) {
                            std::cout << "Exiting Game...\n";
                            window.close();
                        }
                    }
                    break;

                case GameState::RankingGame:
                    rankingGame.handleEvent(event);

                    if (event.type == sf::Event::KeyPressed) {
                        if (event.key.code == sf::Keyboard::Escape) {
                            std::cout << "Returning to Main Menu...\n";
                            currentState = GameState::MainMenu; // Transition back to MainMenu
                        }
                    }
                    break;
            }
        }

        // Rendering based on the current state
        window.clear(sf::Color::Black);
        if (currentState == GameState::MainMenu) {
            mainMenu.render(window);
        } else if (currentState == GameState::RankingGame) {
            rankingGame.render(window);
        }
        window.display();
    }

    return 0;
}
