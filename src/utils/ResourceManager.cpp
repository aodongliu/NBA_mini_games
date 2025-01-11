#include "utils/ResourceManager.hpp"
#include <filesystem>
#include <iostream>

ResourceManager& ResourceManager::getInstance() {
    static ResourceManager instance;
    return instance;
}

std::shared_ptr<sf::Font> ResourceManager::loadFont(const std::string& path) {
    auto it = fonts.find(path);
    if (it != fonts.end()) {
        return it->second;
    }

    auto font = std::make_shared<sf::Font>();
    if (!font->loadFromFile(path)) {
        throw ResourceError("Failed to load font: " + path);
    }

    fonts[path] = font;
    return font;
}

bool ResourceManager::loadTexture(const std::string& path, sf::Texture& texture) {
    // Replace .svg with .png in the path
    std::string actualPath = path;
    size_t svgPos = actualPath.find(".svg");
    if (svgPos != std::string::npos) {
        actualPath.replace(svgPos, 4, ".png");
    }

    // Check if texture is already loaded
    auto it = textureCache.find(actualPath);
    if (it != textureCache.end()) {
        texture = it->second;
        return true;
    }

    // Load the texture
    if (!texture.loadFromFile(actualPath)) {
        std::cerr << "Failed to load image \"" << actualPath << "\". ";
        return false;
    }

    // Cache the texture
    textureCache[actualPath] = texture;
    return true;
}

void ResourceManager::clearTextures() {
    textureCache.clear();
} 