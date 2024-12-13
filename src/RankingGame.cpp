#include "RankingGame.hpp"
#include <iostream>
#include <fstream>
#include <random>

void RankingGame::setInstruction(const std::string& message) {
    instructionText.setString(message);
}

void RankingGame::setError(const std::string& message) {
    errorMessage = message;
    errorClock.restart();
}

RankingGame::RankingGame(const sf::Font& font, sf::Vector2u windowSize) 
    : currentPlayerIndex(0), errorClock() {
    
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
    instructionText.setCharacterSize(24);
    instructionText.setFillColor(sf::Color::White);
    instructionText.setPosition(20,20);
    setInstruction("Rank this player (1-10):");

    loadNextPlayer();
}

void RankingGame::drawText(sf::RenderWindow& window, const std::string& content, 
        const sf::Font& font, unsigned int size, sf::Vector2f position, sf::Color color) const {

    sf::Text text;
    text.setFont(font);
    text.setString(content);
    text.setCharacterSize(size);
    text.setFillColor(color);
    text.setPosition(position);
    window.draw(text);

}

void RankingGame::loadNextPlayer() {
    if (currentPlayerIndex >= players.size() ) {
        setInstruction("Ranking complete! Press ESC to return to the menu.");
        saveRankingToCSV();
        return;
    }

    const Player& player = players[currentPlayerIndex];
    if (!player.loadImage(currentPlayerTexture)) {
        setError("Failed to load image for player: " + player.getFirstName() + " " + player.getLastName());
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
                setError("Rank already taken. Choose another.");
                return;
            }

            rankings[rank] = std::make_shared<Player>(players[currentPlayerIndex]);
            currentPlayerIndex++;

            if (currentPlayerIndex < players.size()) {
                loadNextPlayer();
            } else {
                setInstruction("Ranking complete! Press ESC to return to the menu.");
                saveRankingToCSV();
            }
        } else if (event.key.code == sf::Keyboard::Escape) {
            setInstruction("Returning to the menu...");
        }
    }
}

void RankingGame::render(sf::RenderWindow& window) {

    window.draw(instructionText); 
 
    if (errorClock.getElapsedTime().asSeconds()< 3.0f && !errorMessage.empty() ) {
        drawText(window, errorMessage, *instructionText.getFont(), 20, sf::Vector2f(20, 60), sf::Color::Red);
    }

    window.draw(currentPlayerSprite);
    displayRankings(window);
}

void RankingGame::displayRankings(sf::RenderWindow& window) {
    float x = window.getSize().x * 0.7f;
    float y = window.getSize().y * 0.1f;

    for (int i = 1; i <= 10; ++i ) {
        std::string content = std::to_string(i) + ": ";

        if (rankings.count(i)){
            content += rankings[i]->getFirstName() + " " + rankings[i]->getLastName();
        } else{
            content += "[Empty]";
        }
        
        drawText(window, content, *instructionText.getFont(), 20, sf::Vector2f(x, y), sf::Color::White);
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