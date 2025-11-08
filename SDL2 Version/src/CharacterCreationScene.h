#pragma once
#include <SDL2/SDL.h>
#include <string>
#include <vector>

class Font;
class Player;

class CharacterCreationScene {
public:
    CharacterCreationScene(SDL_Renderer* renderer, Font& font);
    ~CharacterCreationScene();
    
    void Update(float deltaTime);
    void Render();
    void HandleInput(SDL_Event& e);
    
    bool IsComplete() const { return selectionComplete; }
    std::string GetSelectedState() const { return selectedState; }
    
    void Reset();
    
private:
    SDL_Renderer* renderer;
    Font& font;
    
    std::vector<std::string> states = {
        "Delhi",
        "Bangalore",
        "Mumbai",
        "Kerala",
        "Punjab",
        "Rajasthan"
    };
    
    int selectedIndex = 0;
    std::string selectedState;
    bool selectionComplete = false;
    
    void RenderStateList();
    void RenderStateDescription();
};
