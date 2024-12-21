#include <SFML/Graphics.hpp>
#include "base/enums.hpp"
#include "base/GameEngine.hpp"
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
    
    GameEngine engine(window, font); 
    engine.run();

    return 0;
}
