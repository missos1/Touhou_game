#pragma once
#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <SDL.h>
#include <SDL_image.h>
#include <string>

class TextureManager {
public:
    static SDL_Texture* loadTexture(const std::string& fileName, SDL_Renderer* renderer);
};

#endif
