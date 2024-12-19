#pragma once
#include <SFML/Graphics.hpp>

// Colors obtained from: https://imagecolorpicker.com/color-code/2596be
// suffix T in color naming for transparency (0-255) 
namespace Colors {
    // Used for light themes
    // Added 12/17/2024
    const sf::Color purple(53, 30, 74);  
    const sf::Color orange(210, 131, 31); 
    const sf::Color red(214, 3, 3); 
    const sf::Color creamWhite(230, 223, 218); 
    const sf::Color lightGreenT(218, 230, 218, 200);
    // Added 12/18/2024
    const sf::Color blueGrey(136, 147, 153);
    const sf::Color beigeGrey(209, 205, 186);
    const sf::Color expresso(74, 49, 36);
    const sf::Color silverWhite(241, 240, 236);
    const sf::Color walnut(119, 67, 30);
    const sf::Color starBlue(65, 187, 200);
    const sf::Color rubyBrick(215, 85, 60);

    // Used for dark themes
    // Added 12/17/2024
    const sf::Color paleBlack(67, 61, 61);  
    const sf::Color lightGrey(230, 226, 226);  
    const sf::Color darkYellow(239, 199, 66);  
    const sf::Color darkGreenT(85, 100, 85, 200);
    // Added 12/18/2024
    const sf::Color midnight(32, 33, 43);  
    const sf::Color tan(208, 176, 138);  
    const sf::Color retroNavyT(31, 62, 83, 200);  
    const sf::Color carnation(242, 72, 91);  
}