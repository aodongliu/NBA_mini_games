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
            
            mainMenu.handleEvent(event);

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                int choice = mainMenu.getSelectedOption();
                if (choice == 0) {
                    std::cout << "Starting Ranking Game...\n";
                    // Transition to the ranking game
                } else if (choice == 1) {
                    std::cout << "Exiting Game...\n";
                    window.close();
                }
            }

            if (mainMenu.isOptionDoubleClicked()){
                int choice = mainMenu.getSelectedOption();
                if (choice == 0) {
                    std::cout << "Starting Ranking Game...\n";
                } else if (choice == 1) {
                    std::cout << "Exiting Game...\n";
                    window.close();
                }
            }
        }
        window.clear(sf::Color::Black);
        mainMenu.render(window);
        window.display();
    }
    
    return 0;
}