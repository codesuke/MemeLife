#include "Font.h"
#include <iostream>

Font::Font() : font(nullptr), cachedRenderer(nullptr) {}

Font::~Font() {
    if (font) {
        TTF_CloseFont(font);
        font = nullptr;
    }
}

bool Font::LoadFont(const std::string& fontPath, int pointSize) {
    if (font) {
        TTF_CloseFont(font);
        font = nullptr;
    }

    font = TTF_OpenFont(fontPath.c_str(), pointSize);
    if (!font) {
        std::cerr << "Failed to load font from " << fontPath << ": " << TTF_GetError() << std::endl;
        return false;
    }

    std::cout << "Font loaded successfully: " << fontPath << " at " << pointSize << "pt\n";
    return true;
}

void Font::RenderText(SDL_Renderer* renderer, const std::string& text, int x, int y,
                     int r, int g, int b) {
    if (!font) {
        std::cerr << "Font not loaded! Call LoadFont first.\n";
        return;
    }

    // Skip rendering empty strings
    if (text.empty()) {
        return;
    }

    // Create a color struct
    SDL_Color color = { static_cast<Uint8>(r), static_cast<Uint8>(g), 
                       static_cast<Uint8>(b), 255 };

    // Render text to a surface
    SDL_Surface* textSurface = TTF_RenderText_Blended(font, text.c_str(), color);
    if (!textSurface) {
        std::cerr << "Failed to render text: '" << text << "' - " << TTF_GetError() << std::endl;
        return;
    }

    // Convert surface to texture
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (!textTexture) {
        std::cerr << "Failed to create texture from text: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(textSurface);
        return;
    }

    // Set up destination rect
    SDL_Rect destRect;
    destRect.x = x;
    destRect.y = y;
    destRect.w = textSurface->w;
    destRect.h = textSurface->h;

    // Render the texture
    SDL_RenderCopy(renderer, textTexture, nullptr, &destRect);

    // Clean up
    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);
}

int Font::GetStringWidth(const std::string& text) const {
    if (!font) return 0;
    
    int width = 0;
    TTF_SizeText(font, text.c_str(), &width, nullptr);
    return width;
}

int Font::GetStringHeight(const std::string& text) const {
    if (!font) return 0;
    
    int height = 0;
    TTF_SizeText(font, text.c_str(), nullptr, &height);
    return height;
}
