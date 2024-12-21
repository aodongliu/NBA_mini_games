#pragma once
#include <ui/Button.hpp>
//#include <ui/TextManager.hpp>
#include <utils/ThemeManager.hpp>
#include <base/GameBase.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class MainMenu : public GameBase {

public:
    MainMenu(sf::RenderWindow& window, const sf::Font& font);

    void handleEvent(const sf::Event& event) override;
    void render(sf::RenderWindow& window) override;
    void updateTheme() override;
    
    void addOption(const std::string& optionText, std::function<void()> callback);
    void reset();

private:
    std::vector<Button> menuButtons;
    const sf::Font& font;

    int selectedOption;
    bool selectedOptionTriggered;
    void updateButtonThemes();
};