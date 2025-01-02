#pragma once

#include <stdexcept>
#include <string>
#include <system_error>

namespace ErrorHandling {
    // Custom exception classes
    class ConfigError : public std::runtime_error {
    public:
        explicit ConfigError(const std::string& msg) 
            : std::runtime_error("Configuration Error: " + msg) {}
    };

    class FileError : public std::runtime_error {
    public:
        explicit FileError(const std::string& msg)
            : std::runtime_error("File Error: " + msg) {}
    };

    class ResourceError : public std::runtime_error {
    public:
        explicit ResourceError(const std::string& msg)
            : std::runtime_error("Resource Error: " + msg) {}
    };

    // Error checking utilities
    inline void checkFilePath(const std::string& path) {
        if (path.empty()) {
            throw ConfigError("Empty file path");
        }
        // Add more checks as needed
    }

    inline void checkVersion(int major, int minor) {
        if (major < 0 || minor < 0) {
            throw ConfigError("Invalid version number");
        }
    }

    // System error utilities
    inline void throwSystemError(int errCode, const std::string& msg) {
        throw std::system_error(errCode, std::system_category(), msg);
    }
}
