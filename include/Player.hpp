#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Player{
public:
    Player(const std::string& firstName, const std::string& lastName);

    const std::string& getFirstName( ) const { return firstName; };
    const std::string& getLastName() const { return lastName; };
    const std::string& getImagePath() const { return imagePath; };

    bool loadImage(sf::Texture& texture) const;


private:
    std::string firstName;
    std::string lastName;
    std::string imagePath;

    void constructImagePath();

};