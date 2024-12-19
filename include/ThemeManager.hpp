#pragma once

#include <SFML/Graphics.hpp>
#include <colors.hpp>
#include <enums.hpp>
#include "config.hpp"
#include <iostream>

// Singleton class that handles theme colors
class ThemeManager {
public:
    static ThemeManager& getInstance();
    
    const ThemeConfig& getThemeConfig() const { return themeConfig; }
    const ThemeConfig& getLightThemeConfig() const { return lightThemeConfig; }
    const ThemeConfig& getDarkThemeConfig() const { return darkThemeConfig; }
    void toggleTheme(Theme theme);

private:
    ThemeManager();
    ThemeConfig lightThemeConfig;
    ThemeConfig darkThemeConfig;
    ThemeConfig themeConfig;
    Theme currentTheme;

    void setLightThemeConfig();
    void setDarkThemeConfig();

    // Disable copy and assignment
    ThemeManager(const ThemeManager&) = delete;
    ThemeManager& operator=(const ThemeManager&) = delete;
};
