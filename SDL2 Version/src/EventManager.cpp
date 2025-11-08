#include "EventManager.h"
#include "Player.h"
#include "UIManager.h"
#include "AudioManager.h"
#include "GameManager.h"
#include <iostream>
#include <fstream>
#include <sstream>

EventManager::EventManager() {
    InitializeEvents();
}

EventManager::~EventManager() {
}

void EventManager::InitializeEvents() {
    // Define interactable objects on the map
    events.push_back({
        "EV_BUY_COFFEE",
        {200, 200, 32, 32},  // Trigger area
        "EV_BUY_COFFEE",     // Event to trigger
        "There's a coffee stand here. Buy a coffee?"
    });
    
    events.push_back({
        "EV_GET_MONEY",
        {500, 300, 32, 32},  // Trigger area
        "EV_GET_MONEY",      // Event to trigger
        "An ATM! Need some cash?"
    });
}

void EventManager::TriggerEvent(const std::string& eventId, Player& player, UIManager& uiManager, AudioManager& audioManager, GameManager* gameManager) {
    std::cout << "Triggering event: " << eventId << std::endl;
    ParseEventFile(eventId, player, uiManager, audioManager, gameManager);
}

void EventManager::ParseEventFile(const std::string& eventId, Player& player, UIManager& uiManager, AudioManager& audioManager, GameManager* gameManager) {
    // Construct file path
    std::string filePath = "../assets/events/" + eventId + ".txt";
    
    std::ifstream eventFile(filePath);
    if (!eventFile.is_open()) {
        std::cerr << "Failed to open event file: " << filePath << std::endl;
        return;
    }
    
    std::string line;
    while (std::getline(eventFile, line)) {
        // Skip empty lines and comments
        if (line.empty() || line[0] == '#') continue;
        
        // Parse command and arguments
        size_t spacePos = line.find(' ');
        std::string command;
        std::string args;
        
        if (spacePos != std::string::npos) {
            command = line.substr(0, spacePos);
            args = line.substr(spacePos + 1);
        } else {
            command = line;
            args = "";
        }
        
        // Execute the command
        ExecuteCommand(command, args, player, uiManager, audioManager, gameManager);
    }
    
    eventFile.close();
}

void EventManager::ExecuteCommand(const std::string& command, const std::string& args, 
                                   Player& player, UIManager& uiManager, AudioManager& audioManager, GameManager* gameManager) {
    if (command == "DIALOGUE") {
        // Extract text between quotes
        size_t start = args.find('"');
        size_t end = args.rfind('"');
        if (start != std::string::npos && end != std::string::npos && start < end) {
            std::string dialogue = args.substr(start + 1, end - start - 1);
            uiManager.ShowDialogue(dialogue);
        }
    }
    else if (command == "ADD_MONEY") {
        int amount = std::stoi(args);
        player.AddMoney(amount);
        std::cout << "Added " << amount << " money" << std::endl;
    }
    else if (command == "SUBTRACT_MONEY") {
        int amount = std::stoi(args);
        player.AddMoney(-amount);
        std::cout << "Subtracted " << amount << " money" << std::endl;
    }
    else if (command == "ADD_STAT") {
        // Format: ADD_STAT energy 15
        std::istringstream iss(args);
        std::string statName;
        int amount;
        iss >> statName >> amount;
        
        if (statName == "energy") {
            player.AddEnergy(amount);
        } else if (statName == "clout") {
            player.AddClout(amount);
        } else if (statName == "logic") {
            player.AddLogic(amount);
        } else if (statName == "sanity") {
            player.AddSanity(amount);
        }
        std::cout << "Added " << amount << " to " << statName << std::endl;
    }
    else if (command == "PLAY_SOUND") {
        std::cout << "Playing sound: " << args << std::endl;
        // audioManager.PlaySoundEffect(args);  // For future implementation
    }
    else if (command == "HOSPITALIZE") {
        if (gameManager) {
            gameManager->GoToHospital();
        } else {
            std::cerr << "Cannot hospitalize: GameManager not available" << std::endl;
        }
    }
    else {
        std::cerr << "Unknown event command: " << command << std::endl;
    }
}

std::string EventManager::GetDialogueForEvent(const std::string& eventId) const {
    for (const auto& event : events) {
        if (event.eventIdToTrigger == eventId) {
            return event.dialogueText;
        }
    }
    return "";
}
