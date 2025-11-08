#pragma once
#include <SDL2/SDL.h>
#include <string>
#include <vector>

class Player;
class UIManager;
class AudioManager;
class GameManager;

struct Interactable {
    std::string id;
    SDL_Rect triggerArea;
    std::string eventIdToTrigger;
    std::string dialogueText;
};

class EventManager {
public:
    EventManager();
    ~EventManager();
    
    void TriggerEvent(const std::string& eventId, Player& player, UIManager& uiManager, AudioManager& audioManager, GameManager* gameManager = nullptr);
    std::string GetDialogueForEvent(const std::string& eventId) const;
    
    static constexpr int INTERACTION_RANGE = 50;  // Pixels around player

private:
    std::vector<Interactable> events;
    
    void InitializeEvents();
    void ParseEventFile(const std::string& eventId, Player& player, UIManager& uiManager, AudioManager& audioManager, GameManager* gameManager = nullptr);
    void ExecuteCommand(const std::string& command, const std::string& args, Player& player, UIManager& uiManager, AudioManager& audioManager, GameManager* gameManager = nullptr);
};
