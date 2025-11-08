#include "CharacterCreationScene.h"
#include "Font.h"
#include "Buff.h"
#include <iostream>

CharacterCreationScene::CharacterCreationScene(SDL_Renderer* renderer, Font& font)
    : renderer(renderer), font(font), selectedIndex(0), selectionComplete(false) {
}

CharacterCreationScene::~CharacterCreationScene() {
}

void CharacterCreationScene::Update(float deltaTime) {
    // No animation needed for now
}

void CharacterCreationScene::Render() {
    // Clear screen
    SDL_SetRenderDrawColor(renderer, 20, 20, 40, 255);
    SDL_RenderClear(renderer);
    
    // Render title
    font.RenderText(renderer, "CHOOSE YOUR ORIGIN", 300, 50, 255, 200, 100);
    
    // Render state list and description
    RenderStateList();
    RenderStateDescription();
    
    // Render instructions
    font.RenderText(renderer, "UP/DOWN to navigate  |  ENTER to select", 200, 700, 150, 150, 150);
}

void CharacterCreationScene::RenderStateList() {
    int startY = 150;
    int lineHeight = 50;
    
    for (int i = 0; i < states.size(); i++) {
        int y = startY + i * lineHeight;
        int r = 255, g = 255, b = 255;
        
        if (i == selectedIndex) {
            // Highlight selected
            r = 0;
            g = 255;
            b = 0;
            // Draw selection box
            SDL_SetRenderDrawColor(renderer, 50, 100, 50, 255);
            SDL_Rect boxRect = {100, y - 5, 300, 40};
            SDL_RenderFillRect(renderer, &boxRect);
        }
        
        std::string prefix = (i == selectedIndex) ? "> " : "  ";
        font.RenderText(renderer, prefix + states[i], 120, y, r, g, b);
    }
}

void CharacterCreationScene::RenderStateDescription() {
    Buff buff = BuffDatabase::GetBuffForState(states[selectedIndex]);
    
    int descX = 500;
    int descY = 150;
    
    // Render buff name
    font.RenderText(renderer, buff.name, descX, descY, 255, 255, 0);
    
    // Render description (multi-line)
    std::string desc = buff.description;
    size_t lineStart = 0;
    int lineY = descY + 50;
    const int lineHeight = 30;
    
    while (lineStart < desc.length()) {
        size_t lineEnd = desc.find('\n', lineStart);
        if (lineEnd == std::string::npos) {
            lineEnd = desc.length();
        }
        
        std::string line = desc.substr(lineStart, lineEnd - lineStart);
        font.RenderText(renderer, line, descX, lineY, 200, 200, 200);
        
        lineY += lineHeight;
        lineStart = lineEnd + 1;
    }
    
    // Render stat modifiers
    int statsY = descY + 200;
    font.RenderText(renderer, "Modifiers:", descX, statsY, 100, 200, 255);
    
    statsY += 40;
    if (buff.cloutModifier != 1.0f) {
        std::string text = "Clout: " + std::to_string(static_cast<int>(buff.cloutModifier * 100)) + "%";
        font.RenderText(renderer, text, descX, statsY, 255, 150, 150);
        statsY += 30;
    }
    if (buff.logicModifier != 1.0f) {
        std::string text = "Logic: " + std::to_string(static_cast<int>(buff.logicModifier * 100)) + "%";
        font.RenderText(renderer, text, descX, statsY, 150, 200, 255);
        statsY += 30;
    }
    if (buff.moneyModifier != 1.0f) {
        std::string text = "Money: " + std::to_string(static_cast<int>(buff.moneyModifier * 100)) + "%";
        font.RenderText(renderer, text, descX, statsY, 255, 255, 150);
        statsY += 30;
    }
    if (buff.energyModifier != 1.0f) {
        std::string text = "Energy: " + std::to_string(static_cast<int>(buff.energyModifier * 100)) + "%";
        font.RenderText(renderer, text, descX, statsY, 150, 255, 150);
        statsY += 30;
    }
    if (buff.energyDrainModifier != 1.0f) {
        std::string text = "Energy Drain: " + std::to_string(static_cast<int>(buff.energyDrainModifier * 100)) + "%";
        font.RenderText(renderer, text, descX, statsY, 255, 200, 150);
        statsY += 30;
    }
}

void CharacterCreationScene::HandleInput(SDL_Event& e) {
    if (e.type == SDL_KEYDOWN) {
        if (e.key.keysym.sym == SDLK_UP) {
            selectedIndex = (selectedIndex - 1 + states.size()) % states.size();
        }
        else if (e.key.keysym.sym == SDLK_DOWN) {
            selectedIndex = (selectedIndex + 1) % states.size();
        }
        else if (e.key.keysym.sym == SDLK_RETURN || e.key.keysym.sym == SDLK_e) {
            selectedState = states[selectedIndex];
            selectionComplete = true;
            std::cout << "Selected state: " << selectedState << std::endl;
        }
    }
}

void CharacterCreationScene::Reset() {
    selectedIndex = 0;
    selectedState = "";
    selectionComplete = false;
}
