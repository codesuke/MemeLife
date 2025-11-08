#include "DialogueBox.h"
#include <iostream>

DialogueBox::DialogueBox(const std::string& text, Font& font, SDL_Renderer* renderer)
    : fullText(text), visibleText(""), font(font), typewriterTimer(0.0f), characterIndex(0) {
    
    // Set up dialogue box position (bottom center of screen)
    boxPosition.x = BOX_PADDING;
    boxPosition.y = 768 - BOX_HEIGHT - BOX_PADDING;  // 768 is WINDOW_HEIGHT
    boxPosition.w = 1024 - 2 * BOX_PADDING;           // 1024 is WINDOW_WIDTH
    boxPosition.h = BOX_HEIGHT;
}

DialogueBox::~DialogueBox() {
}

void DialogueBox::Update(float deltaTime) {
    if (characterIndex < fullText.length()) {
        typewriterTimer += deltaTime;
        
        while (typewriterTimer >= TIME_PER_CHARACTER && characterIndex < fullText.length()) {
            visibleText += fullText[characterIndex];
            characterIndex++;
            typewriterTimer -= TIME_PER_CHARACTER;
        }
    }
}

bool DialogueBox::IsFullyDisplayed() const {
    return characterIndex >= fullText.length();
}

void DialogueBox::AdvanceToNextLine() {
    if (!IsFullyDisplayed()) {
        // Skip to end of current text
        visibleText = fullText;
        characterIndex = fullText.length();
    }
}

void DialogueBox::DrawDialogueBox(SDL_Renderer* renderer) {
    // Draw semi-transparent background
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 200);
    SDL_Rect outerBox = boxPosition;
    SDL_RenderFillRect(renderer, &outerBox);
    
    // Draw border
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderDrawRect(renderer, &outerBox);
}

void DialogueBox::Render(SDL_Renderer* renderer) {
    // Draw the dialogue box background
    DrawDialogueBox(renderer);
    
    // Draw text with padding
    int textX = boxPosition.x + BOX_PADDING;
    int textY = boxPosition.y + BOX_PADDING;
    
    // Render the visible text using the font
    font.RenderText(renderer, visibleText, textX, textY, 255, 255, 255);
    
    // Draw continue indicator (blinking arrow) if text is fully displayed
    if (IsFullyDisplayed()) {
        // Draw a small indicator
        int arrowX = boxPosition.x + boxPosition.w - BOX_PADDING - 16;
        int arrowY = boxPosition.y + boxPosition.h - BOX_PADDING - 12;
        
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        SDL_Rect arrow = {arrowX, arrowY, 12, 8};
        SDL_RenderFillRect(renderer, &arrow);
    }
}
