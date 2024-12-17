#pragma once
#include <SFML/Graphics.hpp>

// Colors obtained from: https://imagecolorpicker.com/color-code/2596be
// suffix T in color naming for transparency (0-255) 
namespace Colors {
    // Used for light themes
    const sf::Color purple(53, 30, 74);  
    const sf::Color orange(210, 131, 31); 
    const sf::Color red(214, 3, 3); 
    const sf::Color creamWhite(230, 223, 218); 
    const sf::Color lightGreenT(218, 230, 218, 200);

    // Used for dark themes
    const sf::Color paleBlack(67, 61, 61);  
    const sf::Color lightGrey(230, 226, 226);  
    const sf::Color darkYellow(239, 199, 66);  
    const sf::Color darkGreenT(85, 100, 85, 200);
}