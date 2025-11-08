#pragma once
#include <SDL2/SDL.h>
#include <string>

class Font;
class Player;

class HUDManager {
public:
    HUDManager(Font& font, SDL_Renderer* renderer);
    ~HUDManager();

    void RenderHUD(const Player& player);

private:
    Font& font;
    SDL_Renderer* renderer;
    
    static constexpr int HUD_PADDING = 10;
    static constexpr int HUD_LINE_HEIGHT = 20;
    static constexpr int ENERGY_BAR_WIDTH = 100;
    static constexpr int ENERGY_BAR_HEIGHT = 10;
    
    void DrawEnergyBar(const Player& player, int x, int y);
};
