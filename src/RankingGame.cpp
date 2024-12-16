#include "RankingGame.hpp"
#include <iostream>
#include <fstream>
#include <random>

RankingGame::RankingGame(const sf::Font& font, sf::Vector2u windowSize) 
    : currentPlayerIndex(0), errorClock(), quitConfirmation(false) {

    instructionText.setFont(font);
    instructionText.setCharacterSize(24);
    instructionText.setFillColor(sf::Color::White);
    instructionText.setPosition(20,20);

    resetGame();
}

void RankingGame::setInstruction(const std::string& message) {
    instructionText.setString(message);
}

void RankingGame::setError(const std::string& message) {
    errorMessage = message;
    errorClock.restart();
}

void RankingGame::resetGame() {
    rankings.clear();
    players.clear();
    currentPlayerIndex = 0;
    quitConfirmation = false;
    subGameState = SubGameState::Running;

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

    setInstruction("Rank this player (1-10). Press Enter to confirm, ESC to quit.");
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

bool RankingGame::isValidInput(const std::string& input, int& rank, std::string& errorMsg) {
    if (input.empty()) {
        errorMsg = "Input cannot be empty! Please enter a number between 1 and 10.";
        return false;
    }

    for (char c : input) {
        if (!std::isdigit(c)) {
            errorMsg = "Invalid input! Please enter a valid number between 1 and 10.";
            return false;
        }
    }

    rank = std::stoi(input);

    if (rank < 1 || rank > 10) {
        errorMsg = "Invalid rank! Please enter a number between 1 and 10.";
        return false;
    }

    errorMsg.clear();
    return true;
}

void RankingGame::handleEvent(const sf::Event& event){
    if (quitConfirmation) {
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
            setInstruction("Exiting to the menu...");
            subGameState = SubGameState::Ended;
        } else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
            quitConfirmation = false;
            setInstruction("Rank this player (1-10). Press Enter to confirm, ESC to quit.");
        }
        return;
    }

    if (event.type == sf::Event::TextEntered) {
        if (std::isdigit(event.text.unicode)) {
            userInput += static_cast<char>(event.text.unicode);
        } else if (event.text.unicode == '\b' && !userInput.empty() ){
            userInput.pop_back();
        }
    } else if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Enter) {
            int rank;
            std::string errorMsg;
            if (isValidInput(userInput, rank, errorMsg)) {
                rankings[rank] = std::make_shared<Player>(players[currentPlayerIndex]);
                currentPlayerIndex++;
                if (currentPlayerIndex < players.size()) {
                    loadNextPlayer();
                } else {
                    setInstruction("Ranking complete! Press ESC to return to the menu.");
                    saveRankingToCSV();
                }
                userInput.clear();
            } else {
                setError(errorMsg);
            }
        } else if (event.key.code == sf::Keyboard::Escape) {
            quitConfirmation = true;
            setInstruction("Are you sure you want to quit? Press Enter to confirm, ESC to cancel.");
        }
    }

}

void RankingGame::render(sf::RenderWindow& window) {

    window.draw(instructionText); 

    drawText(window, "Your Input (press enter to confirm): " + userInput, *instructionText.getFont(), 20, sf::Vector2f(20, 100), sf::Color::Yellow);

    if (errorClock.getElapsedTime().asSeconds()< 3.0f && !errorMessage.empty() ) {
        drawText(window, errorMessage, *instructionText.getFont(), 20, sf::Vector2f(20, 60), sf::Color::Red);
    }

    window.draw(currentPlayerSprite);
    displayRankings(window);
}

void RankingGame::displayRankings(sf::RenderWindow& window) {
    float x = window.getSize().x * 0.7f;
    float y = window.getSize().y * 0.1f;
    float width  = 250;                   // Fixed width
    float height = 350; 

    sf::RectangleShape background(sf::Vector2f(width, height));
    background.setFillColor(sf::Color(50, 50, 50, 200)); 
    background.setOutlineColor(sf::Color::White);        
    background.setOutlineThickness(2);
    background.setPosition(x - 20, y - 20);              
    window.draw(background);


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