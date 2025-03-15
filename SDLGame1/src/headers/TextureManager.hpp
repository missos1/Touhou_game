#ifndef TEXTUREMANAGER_HPP
#define TEXTUREMANAGER_HPP

#include <SDL.h>
#include <SDL_image.h>
#include <unordered_map>
#include <string>

class Game;

class TextureManager {
public:
    static SDL_Texture* LoadTexture(const char* fileName); // get texture from files
    static void cleanup(); // clean up

private:
    static std::unordered_map<std::string, SDL_Texture*> texture_Map; // map to store all texture
};

#endif
