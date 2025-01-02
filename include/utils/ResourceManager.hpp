#pragma once

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <unordered_map>
#include <memory>
#include <string>
#include "ErrorHandler.hpp"

class ResourceManager {
public:
    static ResourceManager& getInstance() {
        static ResourceManager instance;
        return instance;
    }

    // Load and cache a font
    std::shared_ptr<sf::Font> loadFont(const std::string& path) {
        ErrorHandling::checkFilePath(path);
        
        auto it = fonts.find(path);
        if (it != fonts.end()) {
            return it->second;
        }

        auto font = std::make_shared<sf::Font>();
        if (!font->loadFromFile(path)) {
            throw ErrorHandling::ResourceError("Failed to load font: " + path);
        }

        fonts[path] = font;
        return font;
    }

    // Load and cache a texture
    std::shared_ptr<sf::Texture> loadTexture(const std::string& path) {
        ErrorHandling::checkFilePath(path);
        
        auto it = textures.find(path);
        if (it != textures.end()) {
            return it->second;
        }

        auto texture = std::make_shared<sf::Texture>();
        if (!texture->loadFromFile(path)) {
            throw ErrorHandling::ResourceError("Failed to load texture: " + path);
        }

        textures[path] = texture;
        return texture;
    }

    // Clear all cached resources
    void clear() {
        fonts.clear();
        textures.clear();
    }

private:
    ResourceManager() = default;
    ~ResourceManager() = default;

    std::unordered_map<std::string, std::shared_ptr<sf::Font>> fonts;
    std::unordered_map<std::string, std::shared_ptr<sf::Texture>> textures;

    // Delete copy constructor and assignment operator
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;
};
