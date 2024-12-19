#pragma once

#include <SFML/Graphics.hpp>
#include <colors.hpp>
#include <enums.hpp>
#include "config.hpp"
#include <iostream>
#include <Button.hpp>

class ThemeManager {
public:
    static ThemeManager& getInstance() {
        static ThemeManager instance;
        return instance;
    }

    void applyTheme(Theme theme);
    Theme getCurrentTheme() const { return currentTheme; }
    const ThemeConfig& getThemeConfig() const { return themeConfig; }

    void handleThemeToggle(const sf::Event& event, const sf::Vector2i& mousePos);
    void renderThemeButton(sf::RenderWindow& window);

private:
    ThemeManager();
    ThemeConfig lightThemeConfig;
    ThemeConfig darkThemeConfig;
    
    Theme currentTheme;
    ThemeConfig themeConfig;

    Button lightButton, darkButton;
    sf::Text lightLabel, darkLabel;
    sf::Font font;

    void defineLightThemeConfig();
    void defineDarkThemeConfig();
    void setupThemeButtons(const sf::Font& font, const sf::Vector2u& windowSize);

    ThemeManager(const ThemeManager&) = delete;
    ThemeManager& operator=(const ThemeManager&) = delete;
    
};
