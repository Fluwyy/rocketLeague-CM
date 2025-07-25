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
    std::cout << "\n:Current directories:\n"
              << "MapDirectory: " << config.mapDir << "\n"
              << "GameDirectory: " << config.gameDir;
}

void interface(conf& currentConfig);

bool checkConfig(const std::string& filename) {
    return std::filesystem::exists(filename);
}

void startUp() {
    conf currentConfig;
    if (checkConfig(configFile)) {
        std::ifstream inFile(configFile);
        if (inFile.is_open()) {
            std::string jsonContent((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
            inFile.close();
            parseConfigFromJson(jsonContent, currentConfig);
            std::cout << "Configuration loaded from " << configFile;
        } else {
            std::cerr << "Error: Could not open " << configFile << " for reading, but it exists.";
            std::cout << "Please enter initial configuration settings:\n";
            std::string mapPath, gamePath;

            std::cout << "Enter initial Map Directory path: ";
            std::cin.ignore(); 
            std::getline(std::cin, mapPath);
            currentConfig.mapDir = mapPath;

            std::cout << "Enter initial Game Directory path: ";
            std::getline(std::cin, gamePath);
            currentConfig.gameDir = gamePath;

            std::ofstream outFile(configFile);
            if (outFile.is_open()) {
                outFile << serializeConfigToJson(currentConfig);
                outFile.close();
                std::cout << "Initial configuration saved to " << configFile;
            } else {
                std::cerr << "Error: Could not save initial configuration to " << configFile;
            }
        }
    } else {
        std::cout << "Config file not found. Please enter initial configuration settings:\n";
        std::string mapPath, gamePath;

        std::cout << "Enter initial Map Directory path: ";
        std::cin.ignore(); 
        std::getline(std::cin, mapPath);
        currentConfig.mapDir = mapPath;

        std::cout << "Enter initial Game Directory path: ";
        std::getline(std::cin, gamePath);
        currentConfig.gameDir = gamePath;

        std::ofstream outFile(configFile);
        if (outFile.is_open()) {
            outFile << serializeConfigToJson(currentConfig);
            outFile.close();
            std::cout << "Created new config file and saved initial configuration to " << configFile;
        } else {
            std::cerr << "Error: Could not create new config file: " << configFile;
            std::cout << "Using initial configuration in memory (could not save to file).";
        }
    }
    display(currentConfig);
    interface(currentConfig);
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
        std::cout << "Configuration updated and saved to " << configFile;
    } else {
        std::cerr << "Error: Could not open " << configFile << " for writing.";
    }
}

void checkFolder(){
    
}

void interface(conf& currentConfig) {
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
        std::cin >> dirChoice;
        if (dirChoice != 0 && dirChoice != 1) {
            std::cout << "Invalid choice. Try again.\n";
            continue;
        }
        std::cout << "Enter new path: ";
        std::cin.ignore();
        std::getline(std::cin, newPath);
        if (newPath.empty()) {
            std::cout << "Path cannot be empty. Try again.\n";
            continue;
        }
        initializeConfig(dirChoice, newPath, currentConfig);
        display(currentConfig);
        std::cout << "\nWould you like to update another directory? (1 for Yes, 0 for No): ";
        std::cin >> choice;
    }
    std::cout << "Exiting configuration update interface.";
}

int main() {
    startUp();
    return 0;
}