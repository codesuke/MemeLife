#include "Player.h"
#include "Map.h"
#include <SDL2/SDL_image.h>
#include <iostream>
#include <cmath>

Player::Player(SDL_Renderer* renderer, const std::string& spritePath, int startX, int startY)
    : renderer(renderer), spriteTexture(nullptr), x(startX), y(startY), width(SPRITE_SIZE), height(SPRITE_SIZE) {
    
    // Load sprite
    SDL_Surface* surface = IMG_Load(spritePath.c_str());
    if (!surface) {
        std::cerr << "Failed to load player sprite! IMG_Error: " << IMG_GetError() << std::endl;
        return;
    }
    
    width = surface->w;
    height = surface->h;
    
    spriteTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    
    if (!spriteTexture) {
        std::cerr << "Failed to create sprite texture! SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }
}

Player::~Player() {
    if (spriteTexture) {
        SDL_DestroyTexture(spriteTexture);
    }
}

void Player::Update(const Map* map) {
    if (!map) return;
    
    const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);
    float nextX = x;
    float nextY = y;
    
    // Handle input
    if (keyboardState[SDL_SCANCODE_UP]) {
        nextY -= SPEED;
    }
    if (keyboardState[SDL_SCANCODE_DOWN]) {
        nextY += SPEED;
    }
    if (keyboardState[SDL_SCANCODE_LEFT]) {
        nextX -= SPEED;
    }
    if (keyboardState[SDL_SCANCODE_RIGHT]) {
        nextX += SPEED;
    }
    
    // Check collision for horizontal movement
    if (!map->IsSolidTileAt((int)nextX, (int)y) && 
        !map->IsSolidTileAt((int)nextX + width - 1, (int)y) &&
        !map->IsSolidTileAt((int)nextX, (int)y + height - 1) &&
        !map->IsSolidTileAt((int)nextX + width - 1, (int)y + height - 1)) {
        x = nextX;
    }
    
    // Check collision for vertical movement
    if (!map->IsSolidTileAt((int)x, (int)nextY) && 
        !map->IsSolidTileAt((int)x + width - 1, (int)nextY) &&
        !map->IsSolidTileAt((int)x, (int)nextY + height - 1) &&
        !map->IsSolidTileAt((int)x + width - 1, (int)nextY + height - 1)) {
        y = nextY;
    }
}

void Player::Render() {
    if (!spriteTexture) {
        return;
    }
    
    SDL_Rect dstRect;
    dstRect.x = (int)x;
    dstRect.y = (int)y;
    dstRect.w = width;
    dstRect.h = height;
    
    SDL_RenderCopy(renderer, spriteTexture, nullptr, &dstRect);
}

void Player::LevelUp() {
    level++;
    age++;
    
    // Grant permanent stat increases
    maxEnergy += 5.0f;
    energy = maxEnergy;  // Refill energy to new maximum
    logic += 1;
    clout += 1;
    
    std::cout << "LEVEL UP! Now level " << level << ", age " << age << std::endl;
    std::cout << "Max Energy: " << maxEnergy << ", Logic: " << logic << ", Clout: " << clout << std::endl;
}
