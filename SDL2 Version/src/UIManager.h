#pragma once
#include <SDL2/SDL.h>
#include <string>
#include <memory>
#include "DialogueBox.h"
#include "Font.h"
#include "HUDManager.h"

class Player;

class UIManager {
public:
    UIManager(Font& font, SDL_Renderer* renderer);
    ~UIManager();

    void ShowDialogue(const std::string& text);
    void HideDialogue();
    
    void Update(float deltaTime);
    void Render(const Player& player);
    
    bool HasActiveDialogue() const;
    void AdvanceDialogue();
    
private:
    Font& font;
    SDL_Renderer* renderer;
    std::unique_ptr<DialogueBox> currentDialogue;
    HUDManager hudManager;
};

