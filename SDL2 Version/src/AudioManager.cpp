#include "AudioManager.h"
#include <iostream>

AudioManager::AudioManager() : currentMusic(nullptr), initialized(false) {
}

AudioManager::~AudioManager() {
    StopMusic();
    if (currentMusic) {
        Mix_FreeMusic(currentMusic);
        currentMusic = nullptr;
    }
}

bool AudioManager::Initialize() {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1) {
        std::cerr << "Failed to initialize SDL_mixer! Mix_Error: " << Mix_GetError() << std::endl;
        return false;
    }
    
    initialized = true;
    std::cout << "AudioManager initialized successfully." << std::endl;
    return true;
}

bool AudioManager::PlayMusic(const std::string& musicPath, int loops) {
    if (!initialized) {
        std::cerr << "AudioManager not initialized!" << std::endl;
        return false;
    }
    
    // Load music
    Mix_Music* music = Mix_LoadMUS(musicPath.c_str());
    if (!music) {
        std::cerr << "Failed to load music: " << musicPath << "! Mix_Error: " << Mix_GetError() << std::endl;
        std::cerr << "Continuing without background music..." << std::endl;
        return false;
    }
    
    // Free previous music if any
    if (currentMusic) {
        Mix_FreeMusic(currentMusic);
    }
    
    currentMusic = music;
    
    // Play music
    if (Mix_PlayMusic(currentMusic, loops) == -1) {
        std::cerr << "Failed to play music! Mix_Error: " << Mix_GetError() << std::endl;
        Mix_FreeMusic(currentMusic);
        currentMusic = nullptr;
        return false;
    }
    
    std::cout << "Now playing: " << musicPath << std::endl;
    return true;
}

void AudioManager::StopMusic() {
    if (Mix_PlayingMusic()) {
        Mix_HaltMusic();
    }
}

void AudioManager::SetMusicVolume(int volume) {
    Mix_VolumeMusic(volume);
}
