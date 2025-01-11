#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <map>
#include <memory>
#include <stdexcept>  // for std::runtime_error

class ResourceManager {
public:
    static ResourceManager& getInstance();

    std::shared_ptr<sf::Font> loadFont(const std::string& path);
    bool loadTexture(const std::string& path, sf::Texture& texture);
    void clearTextures();

private:
    ResourceManager() = default;
    std::map<std::string, sf::Texture> textureCache;
    std::map<std::string, std::shared_ptr<sf::Font>> fonts;
};

// Define a simple exception class for resource loading errors
class ResourceError : public std::runtime_error {
public:
    explicit ResourceError(const std::string& message) 
        : std::runtime_error(message) {}
};
