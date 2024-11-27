CXX = g++
CXXFLAGS = -Iinclude -I/opt/homebrew/include -std=c++17
LDFLAGS = -L/opt/homebrew/lib -lsfml-graphics -lsfml-window -lsfml-system

SRC = src/main.cpp src/PlayerManager.cpp src/UIManager.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = NBA_Ranking_Game

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $^ -o $@ $(LDFLAGS)

%.o: %.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)

clean:
	rm -f $(OBJ) $(TARGET)

