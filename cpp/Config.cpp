//
// Created by nana on 12/13/24.
//
#include "Config.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

using json = nlohmann::json;

bool Config::loadFromFile(const std::string& filePath) {
    try {
        // Open file
        std::ifstream configFile(filePath);
        if (!configFile.is_open()) {
            std::cerr << "Error: Unable to open config file: " << filePath << std::endl;
            return false;
        }

        // Analyze JSON
        json configJson;
        configFile >> configJson;

        // Read Config
        if (configJson.contains("Game")) {
            const auto& gameConfig = configJson["Game"];
            if (gameConfig.contains("BoardSize")) {
                boardSize = gameConfig["BoardSize"].get<int>();
            }
            if (gameConfig.contains("Komi")) {
                komi = gameConfig["Komi"].get<double>();
            }
            if (gameConfig.contains("Handicap")) {
                handicap = gameConfig["Handicap"].get<int>();
            }
            if (gameConfig.contains("Rule")) {
                rule = gameConfig["Rule"].get<std::string>();
            }
        }

        configFile.close();
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error: Failed to load config file: " << e.what() << std::endl;
        return false;
    }

}


// Single instance
Config& Config::getInstance() {
    static Config instance;
    return instance;
}
