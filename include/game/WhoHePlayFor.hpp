#pragma once

#include "base/GameBase.hpp"
#include "game/Player.hpp"
#include "utils/ResourceManager.hpp"
#include "ui/Button.hpp"
#include <vector>
#include <map>
#include <memory>

class WhoHePlayFor : public GameBase {
public:
    WhoHePlayFor(sf::RenderWindow& window, std::shared_ptr<sf::Font> font);

    void handleEvent(const sf::Event& event) override;
    void render(sf::RenderWindow& window) override;
    void resetGame() override;
    void updateTheme() override;
    
private:
    sf::Sprite currentPlayerSprite;
    sf::Texture currentPlayerTexture;
    std::vector<std::pair<std::string, std::shared_ptr<Button>>> teamButtons;
    std::vector<std::unique_ptr<sf::Texture>> buttonTextures;  // Store textures here
    size_t currentPlayerIndex;
    
    void setUpLabels() override;
    void configurePlayerSprite();
    void loadNextPlayer();
    void createTeamButtons();
    void handleTeamSelection(const std::string& selectedTeamId);
};
