#include "ThemeManager.hpp"


ThemeManager::ThemeManager()
    : currentTheme(Theme::Dark) {

    if (!font.loadFromFile(FONT_PATH "/Arial.ttf")) {
        std::cerr << "Error: Failed to load font.\n";
        return;
    }

    defineLightThemeConfig();
    defineDarkThemeConfig();

    applyTheme(currentTheme);
    setupThemeButtons(font, sf::Vector2u(800, 600));

}

void ThemeManager::defineLightThemeConfig() {
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

void ThemeManager::defineDarkThemeConfig() {
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

void ThemeManager::setupThemeButtons(const sf::Font& font, const sf::Vector2u& windowSize) {
    float buttonWidth = 80;
    float buttonHeight = 30;
    float padding = 5;
    float topMargin = 10;

    // Create light button
    lightButton = Button(font, "Light", 
                         sf::Vector2f(windowSize.x - 2 * (buttonWidth + padding), topMargin), 
                         sf::Vector2f(buttonWidth, buttonHeight), 
                         lightThemeConfig.buttonColor, lightThemeConfig.instructionTextColor);

    // Create dark button
    darkButton = Button(font, "Dark", 
                        sf::Vector2f(windowSize.x - (buttonWidth + padding), topMargin), 
                        sf::Vector2f(buttonWidth, buttonHeight), 
                        darkThemeConfig.buttonColor, darkThemeConfig.instructionTextColor);
}

void ThemeManager::applyTheme(Theme theme) {
    currentTheme = theme;
    themeConfig = (theme == Theme::Light) ? lightThemeConfig : darkThemeConfig;

    // Update button themes to match the new theme
    lightButton.setTheme(lightThemeConfig.buttonColor, lightThemeConfig.instructionTextColor, themeConfig.borderColor);
    darkButton.setTheme(darkThemeConfig.buttonColor, darkThemeConfig.instructionTextColor, themeConfig.borderColor);
}

void ThemeManager::handleThemeToggle(const sf::Event& event, const sf::Vector2i& mousePos) {
    // Hover effect handling
    lightButton.updateHoverState(mousePos, lightThemeConfig.highlightAreaColor, lightThemeConfig.highlightTextColor);
    darkButton.updateHoverState(mousePos, darkThemeConfig.highlightAreaColor, darkThemeConfig.highlightTextColor);

    // Handle button clicks
    if (lightButton.isClicked(event, mousePos)) {
        applyTheme(Theme::Light);
    } else if (darkButton.isClicked(event, mousePos)) {
        applyTheme(Theme::Dark);
    }
}

void ThemeManager::renderThemeButton(sf::RenderWindow& window) {
    lightButton.render(window);
    darkButton.render(window);
}