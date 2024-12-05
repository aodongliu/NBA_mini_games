#include "RankingGame.hpp"
#include <iostream>


RankingGame::RankingGame(const sf::Font& font, sf::Vector2u windowSize) {
    instructionText.setFont(font);
    instructionText.setString("Rank this player (1-10):");
    instructionText.setCharacterSize(24);
    instructionText.setFillColor(sf::Color::White);
    instructionText.setPosition(20,20);
    
    
}

void RankingGame::handleEvent(const sf::Event& event){
    std::cout << "RankingGame event handled.\n";
}

void RankingGame::render(sf::RenderWindow& window) {
    window.draw(instructionText); 
}