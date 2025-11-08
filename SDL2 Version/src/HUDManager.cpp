#include "HUDManager.h"
#include "Font.h"
#include "Player.h"
#include <sstream>
#include <iomanip>

HUDManager::HUDManager(Font& font, SDL_Renderer* renderer)
    : font(font), renderer(renderer) {
}

HUDManager::~HUDManager() {
}

void HUDManager::RenderHUD(const Player& player) {
    int x = HUD_PADDING;
    int y = HUD_PADDING;
    
    // Format stats strings
    std::ostringstream cloutStr;
    cloutStr << "Clout: " << player.GetClout();
    font.RenderText(renderer, cloutStr.str(), x, y, 255, 200, 100);
    y += HUD_LINE_HEIGHT;
    
    std::ostringstream logicStr;
    logicStr << "Logic: " << player.GetLogic();
    font.RenderText(renderer, logicStr.str(), x, y, 100, 200, 255);
    y += HUD_LINE_HEIGHT;
    
    std::ostringstream sanityStr;
    sanityStr << "Sanity: " << player.GetSanity() << "%";
    font.RenderText(renderer, sanityStr.str(), x, y, 200, 100, 255);
    y += HUD_LINE_HEIGHT;
    
    std::ostringstream moneyStr;
    moneyStr << "Money: $" << player.GetMoney();
    font.RenderText(renderer, moneyStr.str(), x, y, 100, 255, 100);
    y += HUD_LINE_HEIGHT + 5;
    
    // Draw energy bar
    std::string energyLabel = "Energy:";
    font.RenderText(renderer, energyLabel, x, y, 255, 255, 255);
    DrawEnergyBar(player, x, y + HUD_LINE_HEIGHT);
}

void HUDManager::DrawEnergyBar(const Player& player, int x, int y) {
    // Background rectangle (empty bar)
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    SDL_Rect bgBar = {x, y, ENERGY_BAR_WIDTH, ENERGY_BAR_HEIGHT};
    SDL_RenderFillRect(renderer, &bgBar);
    
    // Foreground rectangle (filled portion)
    float energyPercent = player.GetEnergy() / player.GetMaxEnergy();
    int filledWidth = static_cast<int>(ENERGY_BAR_WIDTH * energyPercent);
    
    // Color based on energy level
    if (energyPercent > 0.5f) {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);  // Green
    } else if (energyPercent > 0.25f) {
        SDL_SetRenderDrawColor(renderer, 255, 200, 0, 255);  // Yellow
    } else {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // Red
    }
    
    SDL_Rect fillBar = {x, y, filledWidth, ENERGY_BAR_HEIGHT};
    SDL_RenderFillRect(renderer, &fillBar);
    
    // Border
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderDrawRect(renderer, &bgBar);
}
