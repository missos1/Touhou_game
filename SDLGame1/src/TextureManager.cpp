#include "headers/TextureManager.hpp"
#include "headers/Game.hpp"
#include <iostream>
#define endl "\n"

std::unordered_map<std::string, SDL_Texture*> TextureManager::texture_Map; // Map to store textures

SDL_Texture* TextureManager::LoadTexture(const char* fileName) {
    std::string fileStr(fileName); // Convert file name to string

    if (texture_Map.find(fileStr) != texture_Map.end()) { // Check if texture is already loaded
        return texture_Map[fileStr]; // Return cached texture
    }

    SDL_Surface* tempSurface = IMG_Load(fileName); // Load image into surface
    if (!tempSurface) {
        std::cout << "Failed to load image: " << IMG_GetError() << endl; // Print error if image loading fails
        return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(Game::Grenderer, tempSurface); // Create texture from surface
    SDL_FreeSurface(tempSurface); // Free the surface

    if (texture) {
        texture_Map[fileStr] = texture; // Store texture in cache
    }

    for (std::pair<const std::string, SDL_Texture*>& pair : texture_Map) {
        std::cout << pair.first << endl; // Debug: print loaded texture IDs
    }

    return texture; // Return the loaded texture
}

void TextureManager::cleanup() {
    for (std::pair<const std::string, SDL_Texture*>& pair : texture_Map) { // Iterate through textures
        SDL_DestroyTexture(pair.second); // Destroy each texture
    }
    texture_Map.clear(); // Clear the texture map
}

