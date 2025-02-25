#ifndef TEXTUREMANAGER_HPP
#define TEXTUREMANAGER_HPP

#include <SDL.h>
#include <SDL_image.h>

class TextureManager {
public:
    static SDL_Texture* LoadTexture(const char* fileName, SDL_Renderer* ren);
};

#endif
