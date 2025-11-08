#ifndef GAMEDATA_H
#define GAMEDATA_H

#include <string>
#include <vector>

using namespace std;

// Stats struct - holds player statistics
struct Stats {
    int clout;
    int logic;
    int sanity;
    int hp;
    int maxHp;
    // Phase 2 additions
    int strength;    // Affects normal attacks and quiz rewards
    int memePower;   // Affects special/finisher potency and growth
};

// Player struct - represents the player character
struct Player {
    string name;
    string memeType;
    int level;
    int xp;
    Stats stats;
};

// Choice struct - represents a choice in a game event
struct Choice {
    string displayText;
    string outcomeId;
};

// GameEvent struct - represents a game event/scenario
struct GameEvent {
    string id;
    string descriptionText;
    vector<Choice> choices;
    vector<string> asciiArt;
};

#endif // GAMEDATA_H
