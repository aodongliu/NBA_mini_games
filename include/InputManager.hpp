#ifndef INPUT_MANAGER_HPP
#define INPUT_MANAGER_HPP

#include <string>
#include <map>
#include <SFML/Graphics.hpp>

class InputManager {
public:
    static bool isValidInput(const std::string& input, int& rank, std::string& errorMsg, const std::map<int, sf::Sprite>& rankings);
};

#endif // INPUT_MANAGER_HPP

