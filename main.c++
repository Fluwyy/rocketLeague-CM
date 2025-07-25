#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>

struct conf {
    std::string mapDir;
    std::string gameDir;
};

std::string configFile = "config.json";

void parseConfigFromJson(const std::string& jsonString, conf& config) {
    size_t mapDirPos = jsonString.find("\"mapDir\": \"");
    if (mapDirPos != std::string::npos) {
        mapDirPos += std::string("\"mapDir\": \"").length();
        size_t mapDirEnd = jsonString.find("\"", mapDirPos);
        if (mapDirEnd != std::string::npos) {
            config.mapDir = jsonString.substr(mapDirPos, mapDirEnd - mapDirPos);
        }
    }
    
    size_t gameDirPos = jsonString.find("\"gameDir\": \"");
    if (gameDirPos != std::string::npos) {
        gameDirPos += std::string("\"gameDir\": \"").length();
        size_t gameDirEnd = jsonString.find("\"", gameDirPos);
        if (gameDirEnd != std::string::npos) {
            config.gameDir = jsonString.substr(gameDirPos, gameDirEnd - gameDirPos);
        }
    }
}

std::string serializeConfigToJson(const conf& config) {
    return "{ \"mapDir\": \"" + config.mapDir + "\", \"gameDir\": \"" + config.gameDir + "\" }";
}

void display(const conf& config) {
    std::cout << "\nCurrent directories:\n"
              << "MapDirectory: " << config.mapDir << "\n"
              << "GameDirectory: " << config.gameDir << "\n";
}

bool checkConfig(const std::string& filename) {
    return std::filesystem::exists(filename);
}

void initializeConfig(int options, const std::string& dir, conf& config) {
    if (options == 0) {
        config.mapDir = dir;
    } else if (options == 1) {
        config.gameDir = dir;
    }
    
    std::ofstream outFile(configFile);
    if (outFile.is_open()) {
        outFile << serializeConfigToJson(config);
        outFile.close();
        std::cout << "Configuration updated and saved to " << configFile << "\n";
    } else {
        std::cerr << "Error: Could not open " << configFile << " for writing.\n";
    }
}

void testCli(conf& currentConfig) {
    int choice;
    std::string newPath;
    
    std::cout << "\nWould you like to update any directory? (1 for Yes, 0 for No): ";
    std::cin >> choice;
    
    while (choice == 1) {
        std::cout << "Select which directory to update:\n";
        std::cout << "0 - Map Directory\n";
        std::cout << "1 - Game Directory\n";
        std::cout << "Enter choice: ";
        
        int dirChoice;
        while (true) {
            std::cin >> dirChoice;
            if (dirChoice != 0 && dirChoice != 1) {
                std::cout << "Invalid choice. Try again.\n";
                continue;
            } else {
                break;
            }
        }
        
        std::cout << "Enter new path: ";
        std::cin >> newPath;
        
        if (!std::filesystem::exists(newPath)) {
            std::cout << "Not valid path\n";
            continue;
        }
        
        initializeConfig(dirChoice, newPath, currentConfig);
        display(currentConfig);
        
        std::cout << "\nWould you like to update another directory? (1 for Yes, 0 for No): ";
        std::cin >> choice;
    }
    
    std::cout << "Exiting configuration update testCli.\n";
}

void startUp() {
    conf currentConfig;
    
    if (!checkConfig(configFile)) {
        std::ofstream outFile(configFile);
        if (outFile.is_open()) {
            outFile << serializeConfigToJson(currentConfig);
            outFile.close();
            std::cout << "Initial configuration saved to " << configFile << "\n";
        } else {
            std::cerr << "Error: Could not save initial configuration to " << configFile << "\n";
        }
    }
    
    std::ifstream inFile(configFile);
    if (inFile.is_open()) {
        std::string jsonContent((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
        inFile.close();
        parseConfigFromJson(jsonContent, currentConfig);
        std::cout << "Configuration loaded from " << configFile << "\n";
    }
    
    display(currentConfig);
    testCli(currentConfig);
}

int main() {
    startUp();
    return 0;
}
