#include "UIManager.h"
#include <iostream>

UIManager::UIManager(Font& font, SDL_Renderer* renderer)
    : font(font), renderer(renderer), currentDialogue(nullptr), hudManager(font, renderer) {
}

UIManager::~UIManager() {
}

void UIManager::ShowDialogue(const std::string& text) {
    currentDialogue = std::make_unique<DialogueBox>(text, font, renderer);
    std::cout << "Dialogue shown: " << text << std::endl;
}

void UIManager::HideDialogue() {
    currentDialogue = nullptr;
}

void UIManager::Update(float deltaTime) {
    if (currentDialogue) {
        currentDialogue->Update(deltaTime);
    }
}

void UIManager::Render(const Player& player) {
    if (currentDialogue) {
        currentDialogue->Render(renderer);
    }
    
    // Always render HUD on top
    hudManager.RenderHUD(player);
}

bool UIManager::HasActiveDialogue() const {
    return currentDialogue != nullptr;
}

void UIManager::AdvanceDialogue() {
    if (currentDialogue) {
        if (currentDialogue->IsFullyDisplayed()) {
            HideDialogue();
        } else {
            currentDialogue->AdvanceToNextLine();
        }
    }
}
