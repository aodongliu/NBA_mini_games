#include <SFML/Graphics.hpp>
#include "PlayerManager.hpp"
#include "UIManager.hpp"
#include "InputManager.hpp"
#include <iostream>
#include <fstream>

int main() {
    sf::RenderWindow window(sf::VideoMode(1200, 800), "NBA Ranking Game"); // Larger window
    PlayerManager playerManager("player_images", 10);
    UIManager uiManager("fonts/Arial.ttf");

    sf::Sprite playerSprite;
    sf::Texture playerTexture;
    std::string userInput;
    std::string errorMsg;
    int rank = -1; // Holds the parsed rank

    if (!playerManager.loadNextPlayer(playerSprite, playerTexture, window.getSize())) {
        std::cerr << "Error loading initial player.\n";
        return -1;
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::TextEntered) {
                if (std::isdigit(event.text.unicode)) {
                    userInput += static_cast<char>(event.text.unicode);
                } else if (event.text.unicode == '\b' && !userInput.empty()) {
                    userInput.pop_back();
                } else if (event.text.unicode == '\r' || event.text.unicode == '\n') {
                    if (InputManager::isValidInput(userInput, rank, errorMsg, playerManager.getRankingSprites())) {
                        sf::Sprite rankedSprite;
                        if (playerManager.assignRank(rank, rankedSprite)) {
                            if (playerManager.isRankingComplete()) {
                                std::cout << "Ranking complete!\n";
                                window.close();
                            } else if (!playerManager.loadNextPlayer(playerSprite, playerTexture, window.getSize())) {
                                std::cerr << "Error loading next player.\n";
                            }
                        }
                        errorMsg.clear();
                    }
                    userInput.clear();
                }
            }
        }

				uiManager.render(window, playerSprite, userInput, playerManager.getRankingSprites(), errorMsg, playerManager.getRankingNames());

    }

    // Write rankings to CSV
    std::ofstream csvFile("rankings.csv");
    if (!csvFile) {
        std::cerr << "Error: Unable to create rankings.csv\n";
        return -1;
    }
    csvFile << "Rank,Player\n";
    for (const auto& [rank, name] : playerManager.getRankingNames()) {
        csvFile << rank << "," << name << "\n";
    }
    csvFile.close();

    std::cout << "Rankings saved to rankings.csv\n";
    return 0;
}

