#pragma once
#include <string>
#include <vector>
#include <memory>
#include <sqlite3.h>

class DatabaseManager {
public:
    // Matches database schema from schema.py exactly
    struct PlayerRecord {
        int id;
        std::string firstName;
        std::string lastName;
        std::string teamId;
        std::string headshotPath;
        float ppg;
        float rpg;
        float apg;
    };

    struct TeamData {
        std::string id;
        std::string name;
        std::string abbreviation;
        std::string logoPath;
    };
