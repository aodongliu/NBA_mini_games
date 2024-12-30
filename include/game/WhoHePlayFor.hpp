#pragma once

#include "base/GameBase.hpp"
#include "game/Player.hpp"
#include <vector>
#include <map>
#include <memory>
class WhoHePlayFor : public GameBase {

public:
    WhoHePlayFor(sf::RenderWindow& window, const sf::Font& font);

    void handleEvent(const sf::Event& event) override;
    void render(sf::RenderWindow& window) override;
    void resetGame() override;
    
private:

    sf::Sprite currentPlayerSprite;
    sf::Texture currentPlayerTexture;

    size_t currentPlayerIndex;
    
    void setUpLabels() override;
    void configurePlayerSprite();
    void loadNextPlayer();
};
