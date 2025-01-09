cmake_minimum_required(VERSION 3.10)
project(NBA_MiniGames VERSION 1.0.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find SFML
find_package(SFML 2.5 REQUIRED COMPONENTS graphics window system)

# Find SQLite3
find_package(PkgConfig REQUIRED)
pkg_check_modules(SQLite3 REQUIRED IMPORTED_TARGET sqlite3)

# Add SFML include directories
include_directories(${SFML_INCLUDE_DIRS})

# Include directories
include_directories(include)

# Configure paths
set(RESOURCES_PATH "${PROJECT_SOURCE_DIR}/resources")
set(FONT_PATH "${RESOURCES_PATH}/fonts")
set(PLAYER_IMAGES_PATH "${RESOURCES_PATH}/player_images")
set(TEAM_LOGOS_PATH "${RESOURCES_PATH}/team_logos")
set(DATABASE_PATH "${RESOURCES_PATH}/data/nba_data.db")

# Generate config header
configure_file(
    "${PROJECT_SOURCE_DIR}/include/utils/config.hpp.in"
    "${PROJECT_SOURCE_DIR}/include/utils/config.hpp"
)

# Gather source files recursively
file(GLOB_RECURSE SOURCES 
    src/*.cpp
    src/base/*.cpp
    src/game/*.cpp
    src/ui/*.cpp
    src/utils/*.cpp
)

# Add executable
add_executable(NBA_MiniGames ${SOURCES})

# Include directories
target_include_directories(NBA_MiniGames PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${SFML_INCLUDE_DIR}
    ${SQLite3_INCLUDE_DIRS}
)

# Link libraries
target_link_libraries(NBA_MiniGames 
    PRIVATE 
    sfml-graphics 
    sfml-window 
    sfml-system
    PkgConfig::SQLite3
)

