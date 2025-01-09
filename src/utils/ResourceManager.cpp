#include "utils/ResourceManager.hpp"
#include <filesystem>

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
    if (!std::filesystem::exists(path)) {
        return false;
    }

    if (!texture.loadFromFile(path)) {
        return false;
    }

    return true;
}

void ResourceManager::clearTextures() {
    textureCache.clear();
} 