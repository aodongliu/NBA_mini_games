# Compiler and Flags
CXX = g++
CXXFLAGS = -Iinclude -I/opt/homebrew/include -std=c++17
LDFLAGS = -L/opt/homebrew/lib -lsfml-graphics -lsfml-window -lsfml-system

# Project Structure
SRC = src/main.cpp src/PlayerManager.cpp src/UIManager.cpp src/InputManager.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = NBA_Ranking_Game

# Default target
all: $(TARGET)

# Linking the executable
$(TARGET): $(OBJ)
	$(CXX) $^ -o $@ $(LDFLAGS)

# Compiling each source file
%.o: %.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)

# Clean up
clean:
	rm -f $(OBJ) $(TARGET)

