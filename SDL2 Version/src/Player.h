#pragma once
#include <SDL2/SDL.h>
#include <string>

class Map;

class Player {
public:
    Player(SDL_Renderer* renderer, const std::string& spritePath, int startX, int startY);
    ~Player();

    void Update(const Map* map);
    void Render();
    
    int GetX() const { return x; }
    int GetY() const { return y; }
    void SetX(int newX) { x = newX; }
    void SetY(int newY) { y = newY; }
    
    // Stats system
    int GetClout() const { return clout; }
    int GetLogic() const { return logic; }
    int GetSanity() const { return sanity; }
    int GetMoney() const { return money; }
    float GetEnergy() const { return energy; }
    float GetMaxEnergy() const { return maxEnergy; }
    int GetLevel() const { return level; }
    int GetAge() const { return age; }
    
    void AddClout(int amount) { clout += amount; }
    void AddLogic(int amount) { logic += amount; }
    void AddSanity(int amount) { sanity = std::min(100, sanity + amount); }
    void AddMoney(int amount) { money = std::max(0, money + amount); }
    void AddEnergy(float amount) { energy = std::min(maxEnergy, energy + amount); }
    void DrainEnergy(float amount) { energy = std::max(0.0f, energy - amount); }
    void LevelUp();
    
    static constexpr int SPRITE_SIZE = 32;
    static constexpr float SPEED = 4.0f;
    static constexpr float ENERGY_DRAIN_PER_HOUR = 4.0f;

private:
    SDL_Renderer* renderer;
    SDL_Texture* spriteTexture;
    float x, y;  // Using float for smooth movement calculations
    int width, height;
    
    // Character stats
    int clout = 5;
    int logic = 5;
    int sanity = 100;
    int money = 500;
    float energy = 100.0f;
    float maxEnergy = 100.0f;
    int level = 1;
    int age = 16;
};

