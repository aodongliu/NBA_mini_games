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

# Set paths for assets
set(FONT_PATH "${PROJECT_SOURCE_DIR}/fonts")

# Generate configuration header file
configure_file(
    ${CMAKE_SOURCE_DIR}/include/config.hpp.in
    ${CMAKE_BINARY_DIR}/include/config.hpp
)

# Add executable
add_executable(NBA_MiniGames ${SOURCES})

# Add include directories for the generated config header
target_include_directories(NBA_MiniGames PRIVATE ${CMAKE_BINARY_DIR}/include)

# Link SFML libraries
target_link_libraries(NBA_MiniGames sfml-graphics sfml-window sfml-system)
