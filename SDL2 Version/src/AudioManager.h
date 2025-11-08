#pragma once
#include <SDL2/SDL_mixer.h>
#include <string>

class AudioManager {
public:
    AudioManager();
    ~AudioManager();

    bool Initialize();
    bool PlayMusic(const std::string& musicPath, int loops = -1);
    void StopMusic();
    void SetMusicVolume(int volume);  // 0-128

private:
    Mix_Music* currentMusic;
    bool initialized;
};
