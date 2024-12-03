cmake_minimum_required(VERSION 3.10)
project(NBA_MiniGames)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find SFML
find_package(SFML 2.5 REQUIRED graphics window system)

# Include directories (optional if you use headers)
include_directories(include)

# Add source files
file(GLOB SOURCES src/*.cpp)

# Add executable
add_executable(NBA_MiniGames ${SOURCES})

# Link SFML libraries
target_link_libraries(NBA_MiniGames sfml-graphics sfml-window sfml-system)
