#include "RankingGame.hpp"
#include <iostream>


RankingGame::RankingGame(const sf::Font& font, sf::Vector2u windowSize) 
    : currentPlayerIndex(0) {
    
    players.emplace_back("LeBron", "James");
    players.emplace_back("Stephen", "Curry");
    players.emplace_back("Kevin", "Durant");

    instructionText.setFont(font);
    instructionText.setString("Rank this player (1-10):");
    instructionText.setCharacterSize(24);
    instructionText.setFillColor(sf::Color::White);
    instructionText.setPosition(20,20);

    loadNextPlayer();
    
}

void RankingGame::loadNextPlayer() {
    if (currentPlayerIndex >= players.size() ) {
        std::cout << "No more players to load. \n";
    }

    const Player& player = players[currentPlayerIndex];
    if (!player.loadImage(currentPlayerTexture)) {
        std::cerr << "Failed to load image for player: " << player.getFirstName() << " " << player.getLastName() << "\n";
        return;
    }

    currentPlayerSprite.setTexture(currentPlayerTexture);
    currentPlayerSprite.setPosition(400, 200);
    currentPlayerIndex++;

}

void RankingGame::handleEvent(const sf::Event& event){
    if (event.type == sf::Event::KeyPressed) {
        loadNextPlayer();
    }
}

void RankingGame::render(sf::RenderWindow& window) {
    window.draw(instructionText); 
    window.draw(currentPlayerSprite);
}