#include "headers/TextureManager.hpp"
#include "headers/Game.hpp"
#include <iostream>

SDL_Texture* TextureManager::LoadTexture(const char* fileName) {
    SDL_Surface* tempSurface = IMG_Load(fileName);
    if (!tempSurface) {
        std::cout << "Failed to load image: " << IMG_GetError() << std::endl;
        return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(Game::Grenderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    return texture;
}


