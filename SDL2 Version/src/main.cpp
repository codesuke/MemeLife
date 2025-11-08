#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <memory>
#include "Map.h"
#include "Player.h"
#include "AudioManager.h"
#include "TimeManager.h"
#include "Font.h"
#include "UIManager.h"
#include "EventManager.h"
#include "GameManager.h"
#include "CharacterCreationScene.h"

const int WINDOW_WIDTH = 1024;
const int WINDOW_HEIGHT = 768;
const int FPS = 60;
const int FRAME_DELAY = 1000 / FPS;

int main(int argc, char* argv[]) {
    // Initialize SDL Core
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        std::cerr << "SDL_image could not initialize! IMG_Error: " << IMG_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer could not initialize! Mix_Error: " << Mix_GetError() << std::endl;
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    if (TTF_Init() < 0) {
        std::cerr << "SDL_ttf could not initialize! TTF_Error: " << TTF_GetError() << std::endl;
        Mix_CloseAudio();
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "MemeLife", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN
    );

    if (window == nullptr) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        TTF_Quit(); Mix_Quit(); IMG_Quit(); SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    if (renderer == nullptr) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window); TTF_Quit(); Mix_Quit(); IMG_Quit(); SDL_Quit();
        return 1;
    }

    AudioManager audioManager;
    audioManager.Initialize();
    audioManager.PlayMusic("../assets/music/theme.wav", -1);
    
    TimeManager timeManager;
    Font font;
    if (!font.LoadFont("../assets/fonts/Jersey10-Regular.ttf", 16)) {
        std::cerr << "Failed to load font!" << std::endl;
        return 1;
    }
    
    EventManager eventManager;
    CharacterCreationScene charCreation(renderer, font);

    std::unique_ptr<Map> map;
    std::unique_ptr<Player> player;
    std::unique_ptr<GameManager> gameManager;
    std::unique_ptr<UIManager> uiManager;

    bool quit = false;
    bool testDialogueShown = false;
    SDL_Event e;
    Uint32 frameStart;
    int frameTime;
    const float FRAME_TIME_SECONDS = 1.0f / FPS;
    GameState currentGameState = GameState::CHARACTER_CREATION;

    while (!quit) {
        frameStart = SDL_GetTicks();

        while (SDL_PollEvent(&e) != 0) {
            if (currentGameState == GameState::CHARACTER_CREATION) {
                charCreation.HandleInput(e);
            }
            else if (currentGameState == GameState::PLAYING) {
                if (e.type == SDL_QUIT) quit = true;
                else if (e.type == SDL_KEYDOWN) {
                    if (e.key.keysym.sym == SDLK_ESCAPE) quit = true;
                    if (e.key.keysym.sym == SDLK_e && uiManager && uiManager->HasActiveDialogue())
                        uiManager->AdvanceDialogue();
                    if (e.key.keysym.sym == SDLK_SPACE)
                        eventManager.TriggerEvent("EV_TOUCH_LIVE_WIRE", *player, *uiManager, audioManager, gameManager.get());
                }
            }
            else if (e.type == SDL_QUIT) quit = true;
        }

        if (currentGameState == GameState::CHARACTER_CREATION) {
            charCreation.Update(FRAME_TIME_SECONDS);
            if (charCreation.IsComplete()) {
                player = std::make_unique<Player>(renderer, "../assets/sprites/player.png", 16 * 32, 12 * 32);
                map = std::make_unique<Map>(renderer, "../assets/tilesets/world_tileset.png");
                uiManager = std::make_unique<UIManager>(font, renderer);
                gameManager = std::make_unique<GameManager>(*player, timeManager, *uiManager);
                currentGameState = GameState::PLAYING;
                testDialogueShown = false;
                charCreation.Reset();
                std::cout << "Game started!" << std::endl;
            }
        }
        else if (currentGameState == GameState::PLAYING && player) {
            player->Update(map.get());
            timeManager.Update(FRAME_TIME_SECONDS);
            uiManager->Update(FRAME_TIME_SECONDS);
            
            if (timeManager.IsNewHour()) {
                player->DrainEnergy(Player::ENERGY_DRAIN_PER_HOUR);
            }
            
            if (gameManager->GetGameState() == GameState::GAME_OVER) {
                currentGameState = GameState::GAME_OVER;
            }
            
            if (!testDialogueShown) {
                uiManager->ShowDialogue("Welcome to MemeLife! Press E to advance, SPACE for test event.");
                testDialogueShown = true;
            }
        }
        else if (currentGameState == GameState::GAME_OVER) {
            quit = true;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        if (currentGameState == GameState::CHARACTER_CREATION) {
            charCreation.Render();
        }
        else if (currentGameState == GameState::PLAYING && player) {
            map->Render();
            player->Render();
            uiManager->Render(*player);
            
            if (gameManager->GetGameState() == GameState::GAME_OVER) {
                gameManager->RenderGameOver(renderer);
                SDL_Surface* gameOverSurface = TTF_RenderText_Blended(
                    font.GetFont(), "GAME OVER", {255, 0, 0, 255}
                );
                if (gameOverSurface) {
                    SDL_Texture* gameOverTexture = SDL_CreateTextureFromSurface(renderer, gameOverSurface);
                    SDL_Rect destRect = {
                        (WINDOW_WIDTH - gameOverSurface->w) / 2,
                        (WINDOW_HEIGHT - gameOverSurface->h) / 2,
                        gameOverSurface->w, gameOverSurface->h
                    };
                    SDL_RenderCopy(renderer, gameOverTexture, nullptr, &destRect);
                    SDL_DestroyTexture(gameOverTexture);
                    SDL_FreeSurface(gameOverSurface);
                }
            }
        }

        SDL_RenderPresent(renderer);

        frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < FRAME_DELAY) {
            SDL_Delay(FRAME_DELAY - frameTime);
        }
    }

    audioManager.StopMusic();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();

    return 0;
}