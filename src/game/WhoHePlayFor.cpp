#include "game/WhoHePlayFor.hpp"
#include "utils/ThemeManager.hpp"
#include <iostream>
#include <random>

// Constants for layout
constexpr float PLAYER_SECTION_HEIGHT_RATIO = 0.4f;
constexpr float TEAMS_SECTION_START_RATIO = 0.45f;
constexpr float DIVISION_ROW_HEIGHT = 40.0f;  // Height for each division row
constexpr float CONFERENCE_GAP = 30.0f;  // Gap between conferences
constexpr int TEAMS_PER_ROW = 5;  // 5 teams per division

WhoHePlayFor::WhoHePlayFor(sf::RenderWindow& window, std::shared_ptr<sf::Font> font)
    : GameBase(window, font)
    , currentPlayerIndex(0)
{
    resetGame();
}

void WhoHePlayFor::resetGame() {
    players.clear();
    currentPlayerIndex = 0;
    quitConfirmation = false;
    gameState = GameState::Running;
    
    setUpLabels();
    loadPlayers(1); // Load one player at a time
    loadNextPlayer();
    createTeamButtons();
}

void WhoHePlayFor::setUpLabels() {
    const auto& theme = ThemeManager::getInstance().getThemeConfig();
    
    instructionLabel = Label(font, 
        "Which team does this player play for? Click the correct team button.",
        {windowSize.x * 0.05f, windowSize.y * 0.05f},
        theme.instructionTextColor, 24, windowSize.x * 0.9f);
    
    errorLabel = Label(font, "", 
        {windowSize.x * 0.05f, windowSize.y * 0.15f},
        theme.warningTextColor, 20);
}

void WhoHePlayFor::loadNextPlayer() {
    if (currentPlayerIndex >= players.size()) {
        loadPlayers(1);
        currentPlayerIndex = 0;
    }

    const Player& player = players[currentPlayerIndex];
    if (!ResourceManager::getInstance().loadTexture(player.getHeadshotPath(), currentPlayerTexture)) {
        errorLabel.setText("Failed to load image for player: " + player.getFirstName() + " " + player.getLastName());
        return;
    }

    configurePlayerSprite();
}

void WhoHePlayFor::configurePlayerSprite() {
    currentPlayerSprite.setTexture(currentPlayerTexture);

    // Scale sprite to fit in the top section
    float maxHeight = windowSize.y * PLAYER_SECTION_HEIGHT_RATIO;
    float scale = maxHeight / currentPlayerTexture.getSize().y * 0.8f; // Reduce size by 20%
    currentPlayerSprite.setScale(scale, scale);

    // Center the sprite horizontally
    float spriteX = (windowSize.x - currentPlayerSprite.getGlobalBounds().width) / 2;
    float spriteY = windowSize.y * 0.10f; // Adjust Y position to avoid overlap
    currentPlayerSprite.setPosition(spriteX, spriteY);
}

void WhoHePlayFor::createTeamButtons() {
    auto& db = DatabaseManager::getInstance();
    auto teams = db.getTeams();
    const auto& theme = ThemeManager::getInstance().getThemeConfig();
    
    // Group teams by conference and division
    std::map<std::string, std::map<std::string, std::vector<DatabaseManager::TeamData>>> groupedTeams;
    for (const auto& team : teams) {
        groupedTeams[team.conference][team.division].push_back(team);
    }

    teamButtons.clear();
    
    // Calculate button dimensions based on window size
    float buttonWidth = (windowSize.x - 100.0f) / TEAMS_PER_ROW;  // 100px total padding
    float buttonHeight = 35.0f;
    float horizontalPadding = 10.0f;
    
    float startY = windowSize.y * TEAMS_SECTION_START_RATIO;
    float currentY = startY;

    // Create buttons for Eastern Conference
    if (groupedTeams.count("Eastern")) {
        int rowIndex = 0;
        for (const auto& [division, divisionTeams] : groupedTeams["Eastern"]) {
            float currentX = 50.0f;  // Start 50px from left edge
            
            for (const auto& team : divisionTeams) {
                auto button = std::make_shared<Button>(
                    font,
                    team.abbreviation,
                    sf::Vector2f(currentX, currentY),
                    sf::Vector2f(buttonWidth - horizontalPadding, buttonHeight),
                    theme.buttonColor,
                    theme.instructionTextColor
                );

                // Load team logo
                sf::Texture* texture = new sf::Texture();  // Store texture dynamically
                if (ResourceManager::getInstance().loadTexture(team.logo_path, *texture)) {
                    button->setIcon(*texture);
                    buttonTextures.push_back(std::unique_ptr<sf::Texture>(texture)); // Store the texture
                } else {
                    std::cerr << "Failed to load texture for team: " << team.name << " from path: " << team.logo_path << std::endl; // Debug message
                }

                button->setCallback([this, teamId = team.id]() {
                    handleTeamSelection(teamId);
                });

                teamButtons.push_back({team.id, button});
                currentX += buttonWidth;
            }
            
            currentY += DIVISION_ROW_HEIGHT;
            rowIndex++;
        }
    }

    // Add gap between conferences
    currentY += CONFERENCE_GAP;

    // Create buttons for Western Conference
    if (groupedTeams.count("Western")) {
        for (const auto& [division, divisionTeams] : groupedTeams["Western"]) {
            float currentX = 50.0f;  // Start 50px from left edge
            
            for (const auto& team : divisionTeams) {
                auto button = std::make_shared<Button>(
                    font,
                    team.abbreviation,
                    sf::Vector2f(currentX, currentY),
                    sf::Vector2f(buttonWidth - horizontalPadding, buttonHeight),
                    theme.buttonColor,
                    theme.instructionTextColor
                );

                // Load team logo
                sf::Texture* texture = new sf::Texture();  // Store texture dynamically
                if (ResourceManager::getInstance().loadTexture(team.logo_path, *texture)) {
                    button->setIcon(*texture);
                    buttonTextures.push_back(std::unique_ptr<sf::Texture>(texture)); // Store the texture
                } else {
                    std::cerr << "Failed to load texture for team: " << team.name << " from path: " << team.logo_path << std::endl; // Debug message
                }

                button->setCallback([this, teamId = team.id]() {
                    handleTeamSelection(teamId);
                });

                teamButtons.push_back({team.id, button});
                currentX += buttonWidth;
            }
            
            currentY += DIVISION_ROW_HEIGHT;
        }
    }
}

void WhoHePlayFor::handleEvent(const sf::Event& event) {
    if (gameState == GameState::Ending) {
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Enter) {
                resetGame();
            } else if (event.key.code == sf::Keyboard::Escape) {
                gameState = GameState::Ended;
            }
        }
        return;
    }

    // Use the existing Button class's click detection
    if (event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        
        for (const auto& [teamId, button] : teamButtons) {
            if (button->isClicked(event, mousePos)) {
                button->triggerCallback();
                break;
            }
        }
    }
}

void WhoHePlayFor::handleTeamSelection(const std::string& selectedTeamId) {
    const Player& currentPlayer = players[currentPlayerIndex];
    
    if (selectedTeamId == currentPlayer.getTeamId()) {
        // Correct answer
        errorLabel.setText("Correct! " + currentPlayer.getFullName() + 
                         " plays for " + currentPlayer.getTeam().name);
        currentPlayerIndex++;
        loadNextPlayer();
    } else {
        // Wrong answer
        errorLabel.setText("Wrong! Try again!");
    }
    errorClock.restart();
}

void WhoHePlayFor::render(sf::RenderWindow& window) {
    // Render labels
    instructionLabel.render(window);
    
    if (errorClock.getElapsedTime().asSeconds() < 3.0f) {
        errorLabel.render(window);
    }

    // Render player sprite
    window.draw(currentPlayerSprite);

    // Render team buttons using existing Button class
    for (const auto& [teamId, button] : teamButtons) {
        button->render(window);
    }
}

void WhoHePlayFor::updateTheme() {
    GameBase::updateTheme();
    const auto& theme = ThemeManager::getInstance().getThemeConfig();
    
    for (const auto& [teamId, button] : teamButtons) {
        button->setDefaultTheme(theme);
    }
}
