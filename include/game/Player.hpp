#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <iostream>
#include "utils/ErrorHandler.hpp"
#include "utils/ResourceManager.hpp"


class Player{
public:
    
    Player() = default;

    Player(const std::string& firstName, const std::string& lastName);

    const std::string& getFirstName( ) const { return firstName; };
    const std::string& getLastName() const { return lastName; };
    const std::string& getImagePath() const { return imagePath; };

    bool loadImage(sf::Texture& texture) const;

    static std::vector<Player> loadPlayersFromCSV();


private:
    std::string firstName;
    std::string lastName;
    std::string imagePath;

    void constructImagePath();

};