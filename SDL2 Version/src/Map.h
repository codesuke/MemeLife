#pragma once
#include <SDL2/SDL.h>
#include <string>
#include <vector>

class Map {
public:
    Map(SDL_Renderer* renderer, const std::string& tilesetPath);
    ~Map();

    void Render();
    bool IsSolidTileAt(int x, int y) const;
    
    static constexpr int TILE_SIZE = 32;
    static constexpr int MAP_WIDTH = 32;
    static constexpr int MAP_HEIGHT = 24;

private:
    SDL_Renderer* renderer;
    SDL_Texture* tilesetTexture;
    
    // Level data: 0 = floor, 1 = wall
    int levelData[MAP_HEIGHT][MAP_WIDTH];
    
    void InitializeLevelData();
};
