#include "InputManager.hpp"
#include <cctype>
#include <sstream>

bool InputManager::isValidInput(const std::string& input, int& rank, std::string& errorMsg, const std::map<int, sf::Sprite>& rankings) {
    // Check if input is empty
    if (input.empty()) {
        errorMsg = "Input cannot be empty! Please enter a number between 1 and 10.";
        return false;
    }

    // Check if input contains only digits
    for (char c : input) {
        if (!std::isdigit(c)) {
            errorMsg = "Invalid input! Please enter a valid number between 1 and 10.";
            return false;
        }
    }

    // Convert the input to an integer
    rank = std::stoi(input);

    // Check if the rank is within the valid range
    if (rank < 1 || rank > 10) {
        errorMsg = "Invalid rank! Please enter a number between 1 and 10.";
        return false;
    }

    // Check if the rank is already taken
    if (rankings.find(rank) != rankings.end()) {
        errorMsg = "Rank " + std::to_string(rank) + " is already taken. Please choose another rank.";
        return false;
    }

    // Input is valid
    errorMsg.clear();
    return true;
}

