#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

class Font {
public:
    Font();
    ~Font();

    bool LoadFont(const std::string& fontPath, int pointSize);
    
    // Render a string at the given position
    void RenderText(SDL_Renderer* renderer, const std::string& text, int x, int y, 
                   int r = 255, int g = 255, int b = 255);
    
    int GetStringWidth(const std::string& text) const;
    int GetStringHeight(const std::string& text) const;
    
    TTF_Font* GetFont() const { return font; }

private:
    TTF_Font* font;
    SDL_Renderer* cachedRenderer;
};
