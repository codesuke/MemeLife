#include <iostream>
#include "GameData.h"
#include "Terminal.h"
#include "AssetLoader.h"

using namespace std;

// Game class - manages the game loop and state
class Game {
private:
    Player player;
    GameEvent currentEvent;
    bool isRunning;
    
public:
    Game() {
        isRunning = true;
        
        // Initialize player with default values
        player.name = "Player";
        player.memeType = "Unknown";
        player.level = 1;
        player.xp = 0;
        player.stats.clout = 50;
        player.stats.logic = 50;
        player.stats.sanity = 50;
        player.stats.hp = 100;
        player.stats.maxHp = 100;
    }
    
    void Run() {
        // Load the starting event
        currentEvent = LoadEvent("01_start");
        
        // Main game loop
        while (isRunning) {
            // Clear the screen
            ClearScreen();
            
            // Move cursor to top
            MoveCursor(0, 0);
            
            // Render the current event description
            cout << currentEvent.descriptionText << endl;
            cout << "\n";
            
            // Display choices if any
            if (!currentEvent.choices.empty()) {
                cout << "Choices:\n";
                for (size_t i = 0; i < currentEvent.choices.size(); i++) {
                    cout << (i + 1) << ". " << currentEvent.choices[i].displayText << endl;
                }
            }
            
            cout << "\nPress any key to continue (or 'q' to quit)...";
            
            // Wait for input
            char input = GetImmediateInput();
            
            // Check if user wants to quit
            if (input == 'q' || input == 'Q') {
                isRunning = false;
            }
        }
        
        // Clear screen on exit
        ClearScreen();
        cout << "Thanks for playing MemeLife!" << endl;
    }
};

int main() {
    Game game;
    game.Run();
    return 0;
}
