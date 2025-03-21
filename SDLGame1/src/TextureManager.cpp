#include "headers/TextureManager.hpp"
#include "headers/Game.hpp"
#include <iostream>
#define endl "\n"

std::unordered_map<std::string, SDL_Texture*> TextureManager::texture_Map;

SDL_Texture* TextureManager::LoadTexture(const char* fileName) {

    std::string fileStr(fileName);

    if(texture_Map.find(fileStr) != texture_Map.end()) { // check if textures are loaded
        return texture_Map[fileStr];
    }

    SDL_Surface* tempSurface = IMG_Load(fileName);
    if (!tempSurface) {
        std::cout << "Failed to load image: " << IMG_GetError() << endl;
        return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(Game::Grenderer, tempSurface);
    SDL_FreeSurface(tempSurface);

    if (texture) {
        texture_Map[fileStr] = texture; // store texture in cache
    }

    for (std::pair<const std::string, SDL_Texture*>& pair : texture_Map) {
        std::cout << pair.first << endl;
    }
    
    return texture;
}

void TextureManager::cleanup() {
    for (std::pair<const std::string, SDL_Texture*>& pair : texture_Map) {
        SDL_DestroyTexture(pair.second);
    }
    texture_Map.clear();
}


