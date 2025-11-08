#pragma once
#include <SDL2/SDL.h>
#include <string>
#include "Font.h"

class DialogueBox {
public:
    DialogueBox(const std::string& text, Font& font, SDL_Renderer* renderer);
    ~DialogueBox();

    void Update(float deltaTime);
    void Render(SDL_Renderer* renderer);
    
    bool IsFullyDisplayed() const;
    void AdvanceToNextLine();
    
    static constexpr int BOX_PADDING = 16;
    static constexpr int BOX_HEIGHT = 120;
    static constexpr float TIME_PER_CHARACTER = 0.05f;  // 50ms per character

private:
    std::string fullText;
    std::string visibleText;
    Font& font;
    
    float typewriterTimer;
    SDL_Rect boxPosition;
    size_t characterIndex;
    
    void DrawDialogueBox(SDL_Renderer* renderer);
};
