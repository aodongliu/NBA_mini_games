#include "ThemeManager.hpp"

// Singleton access
ThemeManager& ThemeManager::getInstance() {
    static ThemeManager instance;
    return instance;
}

ThemeManager::ThemeManager()
    : currentTheme(Theme::Light) {
    setLightThemeConfig();
    setDarkThemeConfig();
    toggleTheme(currentTheme);
}

void ThemeManager::setLightThemeConfig() {
    // Set 1 (12/17/2024):
    // lightThemeConfig.backgroundColor      = Colors::creamWhite;
    // lightThemeConfig.instructionTextColor = Colors::purple;
    // lightThemeConfig.highlightTextColor   = Colors::orange;
    // lightThemeConfig.highlightAreaColor   = Colors::lightGreenT;
    // lightThemeConfig.warningTextColor     = Colors::red; 
    // lightThemeConfig.buttonColor          = Colors::lightGreenT;
    // lightThemeConfig.borderColor          = sf::Color::Black;

    // Set 2 (12/18/2024):
    lightThemeConfig.backgroundColor      = Colors::silverWhite;
    lightThemeConfig.instructionTextColor = Colors::walnut;
    lightThemeConfig.highlightTextColor   = Colors::starBlue;
    lightThemeConfig.highlightAreaColor   = Colors::lightGreenT;
    lightThemeConfig.warningTextColor     = Colors::rubyBrick; 
    lightThemeConfig.buttonColor          = Colors::lightGreenT;
    lightThemeConfig.borderColor          = sf::Color::Black;
}

void ThemeManager::setDarkThemeConfig() {
    // Set 1 (12/17/2024):
    // darkThemeConfig.backgroundColor       = Colors::paleBlack; 
    // darkThemeConfig.instructionTextColor  = Colors::lightGrey; 
    // darkThemeConfig.highlightTextColor    = Colors::darkYellow;  
    // darkThemeConfig.highlightAreaColor    = Colors::darkGreenT;
    // darkThemeConfig.warningTextColor      = Colors::red;
    // darkThemeConfig.buttonColor           = Colors::darkGreenT;
    // darkThemeConfig.borderColor           = sf::Color::White;

    // Set 2 (12/18/2024):
    darkThemeConfig.backgroundColor       = Colors::midnight; 
    darkThemeConfig.instructionTextColor  = Colors::lightGrey; 
    darkThemeConfig.highlightTextColor    = Colors::tan;  
    darkThemeConfig.highlightAreaColor    = Colors::retroNavyT;
    darkThemeConfig.warningTextColor      = Colors::carnation;
    darkThemeConfig.buttonColor           = Colors::retroNavyT;
    darkThemeConfig.borderColor           = sf::Color::White;
}

void ThemeManager::toggleTheme(Theme theme) {
    currentTheme = theme;
    themeConfig = (theme == Theme::Light) ? lightThemeConfig : darkThemeConfig;
}