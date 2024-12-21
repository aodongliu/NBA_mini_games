#include "base/GameBase.hpp"

GameBase::GameBase(sf::RenderWindow& window) 
    : window(window), windowSize(window.getSize()) {}