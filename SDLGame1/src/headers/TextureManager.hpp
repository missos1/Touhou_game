#ifndef TEXTUREMANAGER_HPP
#define TEXTUREMANAGER_HPP

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <unordered_map>
#include <string>

class Game;

class TextureManager {
public:
    static SDL_Texture* LoadTexture(const char* fileName); // get texture from files
    static SDL_Texture* LoadFontTexture(const char* text, TTF_Font* font, SDL_Color color);
    static void render_text(const std::string& text, TTF_Font* font, SDL_Color color, SDL_Texture*& texture, int x, int y);
    static void render_from_texture(SDL_Texture* texture, int x, int y, int multipiler, double angle, SDL_RendererFlip flip);

    static void cleanup(); // clean up

private:
    static std::unordered_map<std::string, SDL_Texture*> texture_Map; // map to store all texture
};

#endif
