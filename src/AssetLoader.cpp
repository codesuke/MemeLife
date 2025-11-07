#include "AssetLoader.h"
#include <fstream>
#include <sstream>

using namespace std;

// Helper function to trim whitespace from a string
string trim(const string& str) {
    size_t first = str.find_first_not_of(' ');
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

// Load a game event from a file
GameEvent LoadEvent(string eventId) {
    GameEvent event;
    string filepath = "assets/events/" + eventId + ".txt";
    ifstream file(filepath);
    
    if (!file.is_open()) {
        // If file can't be opened, return an empty event
        event.id = "error";
        event.descriptionText = "Error: Could not load event " + eventId;
        return event;
    }
    
    string line;
    while (getline(file, line)) {
        // Skip empty lines
        if (line.empty()) continue;
        
        // Check the prefix
        if (line.find("ID:") == 0) {
            // Extract ID
            event.id = trim(line.substr(3));
        }
        else if (line.find("DESC:") == 0) {
            // Extract description
            event.descriptionText = trim(line.substr(5));
        }
        else if (line.find("CHOICE:") == 0) {
            // Extract choice
            string choiceData = trim(line.substr(7));
            size_t separatorPos = choiceData.find('|');
            
            if (separatorPos != string::npos) {
                Choice choice;
                choice.displayText = trim(choiceData.substr(0, separatorPos));
                choice.outcomeId = trim(choiceData.substr(separatorPos + 1));
                event.choices.push_back(choice);
            }
        }
        else if (line.find("ART:") == 0) {
            // Extract ASCII art line
            event.asciiArt.push_back(line.substr(4));
        }
    }
    
    file.close();
    return event;
}
