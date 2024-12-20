cmake_minimum_required(VERSION 3.10)
project(NBA_MiniGames)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find SFML
find_package(SFML 2.5 REQUIRED graphics window system)

# Include directories
include_directories(include)

# Set paths for assets
set(FONT_PATH "${PROJECT_SOURCE_DIR}/fonts")
set(PLAYER_IMAGES_PATH "${PROJECT_SOURCE_DIR}/player_images")
set(PLAYER_LIST_PATH "${PROJECT_SOURCE_DIR}/player_list.csv")

# Generate configuration header file
configure_file(
    ${CMAKE_SOURCE_DIR}/include/utils/config.hpp.in
    ${CMAKE_BINARY_DIR}/include/config.hpp
)

# Gather source files recursively
file(GLOB_RECURSE SOURCES src/*.cpp)

# Add executable
add_executable(NBA_MiniGames ${SOURCES})

# Include directories
target_include_directories(NBA_MiniGames PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_BINARY_DIR}/include
)

# Link SFML libraries
target_link_libraries(NBA_MiniGames sfml-graphics sfml-window sfml-system)

