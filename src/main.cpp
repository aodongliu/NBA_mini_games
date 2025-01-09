#include <SFML/Graphics.hpp>
#include "base/enums.hpp"
#include "base/GameEngine.hpp"
#include "ui/GlobalUI.hpp"
#include "utils/ThemeManager.hpp"
#include "game/MainMenu.hpp"
#include "game/RankingGame.hpp"
#include <iostream>
#include "utils/config.hpp"
#include "utils/ResourceManager.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "NBA Mini Games");

    std::shared_ptr<sf::Font> font;
    try {
        auto& resourceManager = ResourceManager::getInstance();
        font = resourceManager.loadFont(std::string(FONT_PATH) + "/Arial.ttf");
    } catch (const ResourceError& e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }
    
    GameEngine engine(window, font); 
    engine.run();

    return 0;
}
