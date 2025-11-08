#include "Map.h"
#include <SDL2/SDL_image.h>
#include <iostream>

Map::Map(SDL_Renderer* renderer, const std::string& tilesetPath)
    : renderer(renderer), tilesetTexture(nullptr) {
    
    // Load tileset
    SDL_Surface* surface = IMG_Load(tilesetPath.c_str());
    if (!surface) {
        std::cerr << "Failed to load tileset! IMG_Error: " << IMG_GetError() << std::endl;
        return;
    }
    
    tilesetTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    
    if (!tilesetTexture) {
        std::cerr << "Failed to create tileset texture! SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }
    
    InitializeLevelData();
}

Map::~Map() {
    if (tilesetTexture) {
        SDL_DestroyTexture(tilesetTexture);
    }
}

void Map::InitializeLevelData() {
    // Initialize with a simple room layout
    // 0 = floor, 1 = wall
    for (int y = 0; y < MAP_HEIGHT; ++y) {
        for (int x = 0; x < MAP_WIDTH; ++x) {
            // Create walls around the border
            if (x == 0 || x == MAP_WIDTH - 1 || y == 0 || y == MAP_HEIGHT - 1) {
                levelData[y][x] = 1;  // Wall
            }
            // Add some interior walls for variety
            else if ((x == 8 && y > 4 && y < 12) || (y == 12 && x > 4 && x < 16)) {
                levelData[y][x] = 1;  // Wall
            }
            else {
                levelData[y][x] = 0;  // Floor
            }
        }
    }
}

void Map::Render() {
    if (!tilesetTexture) {
        return;
    }
    
    for (int y = 0; y < MAP_HEIGHT; ++y) {
        for (int x = 0; x < MAP_WIDTH; ++x) {
            int tileType = levelData[y][x];
            
            // Source rect: which 32x32 square from the tileset
            // Tileset is 64x32: floor tile at 0-32, wall tile at 32-64
            SDL_Rect srcRect;
            srcRect.w = TILE_SIZE;
            srcRect.h = TILE_SIZE;
            srcRect.y = 0;
            srcRect.x = tileType * TILE_SIZE;  // 0 for floor, 32 for wall
            
            // Destination rect: where on screen to draw
            SDL_Rect dstRect;
            dstRect.x = x * TILE_SIZE;
            dstRect.y = y * TILE_SIZE;
            dstRect.w = TILE_SIZE;
            dstRect.h = TILE_SIZE;
            
            // Draw the tile
            SDL_RenderCopy(renderer, tilesetTexture, &srcRect, &dstRect);
        }
    }
}

bool Map::IsSolidTileAt(int x, int y) const {
    // Convert screen coordinates to grid coordinates
    int gridX = x / TILE_SIZE;
    int gridY = y / TILE_SIZE;
    
    // Check bounds
    if (gridX < 0 || gridX >= MAP_WIDTH || gridY < 0 || gridY >= MAP_HEIGHT) {
        return true;  // Treat out-of-bounds as solid
    }
    
    return levelData[gridY][gridX] == 1;
}
