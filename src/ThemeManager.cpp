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
    setupThemeButtons(sf::Vector2u(800, 600));

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

void ThemeManager::updateButtonStyles() {
    // Light Mode Button
    lightButton.setFillColor(currentTheme == Theme::Light
                                 ? lightThemeConfig.highlightAreaColor  // Highlighted color
                                 : lightThemeConfig.buttonColor);       // Normal color
    lightButton.setOutlineColor(lightThemeConfig.borderColor);

    // Dark Mode Button
    darkButton.setFillColor(currentTheme == Theme::Dark
                                ? darkThemeConfig.highlightAreaColor   // Highlighted color
                                : darkThemeConfig.buttonColor);        // Normal color
    darkButton.setOutlineColor(darkThemeConfig.borderColor);
}

void ThemeManager::setupThemeButtons(const sf::Vector2u& windowSize) {
    float buttonWidth = 80;
    float buttonHeight = 30;
    float padding = 5;
    float topMargin = 10;

    // Light Mode Button
    lightButton.setSize(sf::Vector2f(buttonWidth, buttonHeight));
    lightButton.setPosition(windowSize.x - 2 * (buttonWidth + padding), topMargin);
    lightLabel.setFont(font);
    lightLabel.setString("Light");
    lightLabel.setCharacterSize(14);
    lightLabel.setFillColor(lightThemeConfig.instructionTextColor);
    lightLabel.setPosition(lightButton.getPosition().x + 15, lightButton.getPosition().y + 5);

    // Dark Mode Button
    darkButton.setSize(sf::Vector2f(buttonWidth, buttonHeight));
    darkButton.setPosition(windowSize.x - (buttonWidth + padding), topMargin);
    darkLabel.setFont(font);
    darkLabel.setString("Dark");
    darkLabel.setCharacterSize(14);
    darkLabel.setFillColor(darkThemeConfig.instructionTextColor);
    darkLabel.setPosition(darkButton.getPosition().x + 15, darkButton.getPosition().y + 5);

    updateButtonStyles();
}

void ThemeManager::applyTheme(Theme theme) {
    currentTheme = theme;
    themeConfig = (theme == Theme::Light) ? lightThemeConfig : darkThemeConfig;
    setupThemeButtons(sf::Vector2u(800, 600));
}

void ThemeManager::handleThemeToggle(const sf::Event& event, const sf::Vector2i& mousePos) {
    if (event.type == sf::Event::MouseButtonPressed) {
        if (lightButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
            applyTheme(Theme::Light);
        } else if (darkButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
            applyTheme(Theme::Dark);
        }
    }
}

void ThemeManager::renderThemeButton(sf::RenderWindow& window) {
    window.draw(lightButton);
    window.draw(lightLabel);
    window.draw(darkButton);
    window.draw(darkLabel);
}