#pragma once
#include <SDL2/SDL.h>

enum class GameState {
    MAIN_MENU,
    CHARACTER_CREATION,
    PLAYING,
    GAME_OVER
};

class Player;
class TimeManager;
class UIManager;

class GameManager {
public:
    GameManager(Player& player, TimeManager& timeManager, UIManager& uiManager);
    ~GameManager();
    
    GameState GetGameState() const { return gameState; }
    void SetGameState(GameState state) { gameState = state; }
    
    void GoToHospital();
    void RenderGameOver(SDL_Renderer* renderer);
    
    int GetHospitalizationCount() const { return hospitalizationCount; }
    
private:
    Player& player;
    TimeManager& timeManager;
    UIManager& uiManager;
    
    GameState gameState = GameState::PLAYING;
    int hospitalizationCount = 0;
    
    static constexpr int MAX_HOSPITALIZATIONS = 5;
    static constexpr int BED_X = 16;  // Tile position (16, 12)
    static constexpr int BED_Y = 12;
    static constexpr int HOSPITALIZATION_COST = 1000;
};
