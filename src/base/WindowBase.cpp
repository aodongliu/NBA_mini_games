#include "base/WindowBase.hpp"

WindowBase::WindowBase(sf::RenderWindow& window) 
    : window(window), windowSize(window.getSize()) {}