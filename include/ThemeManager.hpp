#pragma once

#include <SFML/Graphics.hpp>
#include <colors.hpp>
#include <enums.hpp>

class ThemeManager {
public:
    static ThemeManager& getInstance() {
        static ThemeManager instance;
        return instance;
    }

    void applyTheme(Theme theme);
    Theme getCurrentTheme() const { return currentTheme; }
    const ThemeConfig& getThemeConfig() const { return themeConfig; }

    void setupThemeButton(sf::RectangleShape& button, sf::Text& label, const sf::Font& font, const sf::Vector2u& windowSize);
    void handleThemeToggle(const sf::Event& event, const sf::Vector2i& mousePos, sf::RectangleShape& button);
    void renderThemeButton(sf::RenderWindow& window, const sf::RectangleShape& button, const sf::Text& label);

private:
    ThemeManager();
    Theme currentTheme;
    ThemeConfig themeConfig;

    void setLightTheme();
    void setDarkTheme();

    ThemeManager(const ThemeManager&) = delete;
    ThemeManager& operator=(const ThemeManager&) = delete;
    
};
