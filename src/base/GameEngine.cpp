#include "base/GameEngine.hpp"

GameEngine::GameEngine(sf::RenderWindow& window, sf::Font& font)
    : window(window), currentState(nullptr), globalUI(nullptr) {
    // Create each game
    mainMenu = std::make_shared<MainMenu>(window, font);
    rankingGame = std::make_shared<RankingGame>(window, font);
    // Intialize main menu
    mainMenu->addOption("Play Ranking Game", [this]() {
        rankingGame->resetGame();
        setState(rankingGame); // Transition to RankingGame
    });

    mainMenu->addOption("Quit", [&]() {
        window.close(); // Quit the application
    });
    // Set initial state
    currentState = mainMenu;
    currentState->updateTheme();
    
}

void GameEngine::setState(const std::shared_ptr<GameBase>& newState) {
    currentState = newState;
    if (currentState) {
        currentState->updateTheme();
    }
}

void GameEngine::setGlobalUI(GlobalUI* ui) {
    globalUI = ui;
}

void GameEngine::run() {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (globalUI) globalUI->handleEvent(event);
            if (currentState) currentState->handleEvent(event);
        }

        if (currentState == rankingGame && rankingGame->subGameState == SubGameState::Ended) {
            mainMenu->reset();
            setState(mainMenu); 
        }

        // Rendering
        window.clear(ThemeManager::getInstance().getThemeConfig().backgroundColor);

        if (globalUI) globalUI->render(window);
        if (currentState) currentState->render(window);

        window.display();
    }
}