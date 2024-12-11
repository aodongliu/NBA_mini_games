#include "RankingGame.hpp"
#include <iostream>
#include <fstream>
#include <random>

RankingGame::RankingGame(const sf::Font& font, sf::Vector2u windowSize) 
    : currentPlayerIndex(0) {
    
    try {
        players = Player::loadPlayersFromCSV();
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(players.begin(), players.end(), g);
        players.resize(std::min(players.size(), size_t(10)));

        if (players.empty()) {
            throw std::runtime_error("Error: No players found in the CSV.");
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
    }

    instructionText.setFont(font);
    instructionText.setString("Rank this player (1-10):");
    instructionText.setCharacterSize(24);
    instructionText.setFillColor(sf::Color::White);
    instructionText.setPosition(20,20);

    loadNextPlayer();
    
}

void RankingGame::loadNextPlayer() {
    if (currentPlayerIndex >= players.size() ) {
        std::cout << "All players ranked.\n";
        saveRankingToCSV();
        return;
    }

    const Player& player = players[currentPlayerIndex];
    if (!player.loadImage(currentPlayerTexture)) {
        std::cerr << "Failed to load image for player: " << player.getFirstName() << " " << player.getLastName() << "\n";
        return;
    }

    currentPlayerSprite.setTexture(currentPlayerTexture);
    currentPlayerSprite.setPosition(400, 200);

}

void RankingGame::handleEvent(const sf::Event& event){
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code >= sf::Keyboard::Num1 && event.key.code <= sf::Keyboard::Num9) {
            int rank = event.key.code - sf::Keyboard::Num0;

            if (rankings.count(rank)){
                std::cerr << "Rank " << rank << " is already occupied.\n";
                return;
            }

            rankings[rank] = std::make_shared<Player>(players[currentPlayerIndex]);
            currentPlayerIndex++;

            if (currentPlayerIndex < players.size()) {
                loadNextPlayer();
            } else {
                std::cout << "Ranking complete.\n";
                saveRankingToCSV();
            }
        }
    }
}

void RankingGame::render(sf::RenderWindow& window) {
    window.draw(instructionText); 
    window.draw(currentPlayerSprite);
    displayRankings(window);
}

void RankingGame::displayRankings(sf::RenderWindow& window) {
    float x = 500;
    float y = 50;

    for (int i = 1; i <= 10; ++i ) {
        sf::Text rankText;
        rankText.setFont(*instructionText.getFont());
        rankText.setCharacterSize(20);
        rankText.setFillColor(sf::Color::White);
        rankText.setPosition(x,y);

        if (rankings.count(i)){
            rankText.setString(std::to_string(i) + ": " + rankings[i]->getFirstName() + " " + rankings[i]->getLastName());
        } else{
            rankText.setString(std::to_string(i) + ": [Empty]");
        }
        
        window.draw(rankText);
        y += 30;

    }
}

void RankingGame::saveRankingToCSV() {
    std::ofstream file("ranking.csv");
    if (!file) {
        std::cerr << "Failed to save rankings.\n";
        return;
    }

    file << "Rank,First Name,Last Name\n";
    for (const auto& [rank, playerPtr] : rankings) {
        file << rank << "," << playerPtr->getFirstName() << "," << playerPtr->getLastName() << "\n";
    }

    std::cout << "Rankings saved to rankings.csv.\n";

}