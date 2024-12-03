#include <SFML/Graphics.hpp>
#include "MainMenu.hpp"
#include <iostream>
#include "config.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(800,600), "NBA Mini Games");
    

    sf::Font font;
    if (!font.loadFromFile(FONT_PATH "/Arial.ttf")) {
        std::cerr << "Error: Failed to load font. \n";
        return -1;
    }

    MainMenu mainMenu(font, window.getSize());

    while (window.isOpen()){
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear(sf::Color::Black);
        mainMenu.render(window);
        window.display();

        int option = mainMenu.getSelectedOption();
        if (option == 0) {
            std::cout << "Play Ranking Game selected.\n";
            // Transition to the ranking game (to be implemented)
        } else if (option == 1) {
            std::cout << "Quit selected.\n";
            window.close();
        }
    }
    
    return 0;
}