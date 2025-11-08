#include "GameManager.h"
#include "Player.h"
#include "TimeManager.h"
#include "UIManager.h"
#include "Font.h"
#include <iostream>
#include <SDL2/SDL_ttf.h>

GameManager::GameManager(Player& player, TimeManager& timeManager, UIManager& uiManager)
    : player(player), timeManager(timeManager), uiManager(uiManager),
      gameState(GameState::PLAYING), hospitalizationCount(0) {
}

GameManager::~GameManager() {
}

void GameManager::GoToHospital() {
    hospitalizationCount++;
    std::cout << "Hospitalized! Count: " << hospitalizationCount << std::endl;
    
    // Check if game over
    if (hospitalizationCount >= MAX_HOSPITALIZATIONS) {
        gameState = GameState::GAME_OVER;
        std::cout << "GAME OVER! Too many hospitalizations!" << std::endl;
        return;
    }
    
    // Deduct money
    player.AddMoney(-HOSPITALIZATION_COST);
    
    // Reset player position to bed (in pixels)
    player.SetX(BED_X * 32 + 16);
    player.SetY(BED_Y * 32 + 16);
    
    // Refill energy
    player.AddEnergy(player.GetMaxEnergy() - player.GetEnergy());
    
    // Show dialogue
    uiManager.ShowDialogue("You woke up at the hospital. That'll be $1,000...");
    
    std::cout << "Player recovered. Position reset to bed. Money: " << player.GetMoney() << std::endl;
}

void GameManager::RenderGameOver(SDL_Renderer* renderer) {
    if (gameState != GameState::GAME_OVER) {
        return;
    }
    
    // Render semi-transparent dark overlay
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
    SDL_Rect overlay = { 0, 0, 1024, 768 };
    SDL_RenderFillRect(renderer, &overlay);
    
    // Note: Font rendering should be done by caller
    // This just provides the game over state
    std::cout << "Game Over Screen Rendered" << std::endl;
}
