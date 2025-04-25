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

    //for (std::pair<const std::string, SDL_Texture*>& pair : texture_Map) {
    //    std::cerr << pair.first << endl; // Debug: print loaded texture IDs
    //}

    return texture; // Return the loaded texture
}

SDL_Texture* TextureManager::LoadFontTexture(const char* text, TTF_Font* font, SDL_Color color) {
    if (!font) {
        std::cout << "Failed to get font: " << TTF_GetError() << endl;
        return nullptr;
    }

    SDL_Surface* tempTextSurface = TTF_RenderText_Solid(font, text, color);
    if (!tempTextSurface) {
        std::cout << "Failed to load text: " << TTF_GetError() << endl;
        return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(Game::Grenderer, tempTextSurface); // Create texture from surface
    SDL_FreeSurface(tempTextSurface);

    return texture;
}

void TextureManager::cleanup() {
    for (std::pair<const std::string, SDL_Texture*>& pair : texture_Map) { // Iterate through textures
        SDL_DestroyTexture(pair.second); // Destroy each texture
    }
    texture_Map.clear(); // Clear the texture map
}

void TextureManager::render_text(const std::string& text, TTF_Font* font, SDL_Color color, SDL_Texture*& texture, int x, int y) {
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
    texture = LoadFontTexture(text.c_str(), font, color);

    int textW = 0, textH = 0;
    TTF_SizeText(font, text.c_str(), &textW, &textH);

    SDL_Rect destRect = { x, y, textW, textH };
    SDL_RenderCopy(Game::Grenderer, texture, nullptr, &destRect);
}

void TextureManager::render_from_texture(SDL_Texture* texture, int x, int y, int multipiler, double angle, SDL_RendererFlip flip) {
    if (multipiler <= 0) {
        std::cout << "Invalid multiplier" << endl;
        return;
    }

    int textureW = 0, textureH = 0;
    SDL_QueryTexture(texture, nullptr, nullptr, &textureW, &textureH);

    SDL_Rect destRect = { x, y, textureW * multipiler, textureH * multipiler };
    SDL_RenderCopyEx(Game::Grenderer, texture, nullptr, &destRect, angle, nullptr, flip);
}

