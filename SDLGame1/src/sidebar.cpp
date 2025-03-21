#include "headers/sidebar.hpp"
#include "headers/Game.hpp"

Sidebar::Sidebar()
    : tileSize(0), bg_texture(nullptr), title_texture(nullptr),
    destRect{ 0, 0, 0, 0 }, desRect_title{ 0, 0, 0, 0 } {

    bg_texture = TextureManager::LoadTexture("res/BGTEXTURE.png");
    title_texture = TextureManager::LoadTexture("res/SIDEBAR_TITLE.png");

}
Sidebar::~Sidebar() {
    SDL_DestroyTexture(bg_texture);
    SDL_DestroyTexture(title_texture);
}

void Sidebar::render(int winW, int winH) {
    //render background
    int w, h;
    SDL_QueryTexture(bg_texture, nullptr, nullptr, &w, &h); // get texture info
    tileSize = w;
    for (int y = 0; y < winH; y += tileSize) {
        for (int x = 0; x < winW; x += tileSize) {
            if ((x < tileSize * 2) || (x > winW - tileSize * 14)) {   // loop for render "side"bar
                destRect = { x, y, tileSize, tileSize };
                SDL_RenderCopy(Game::Grenderer, bg_texture, nullptr, &destRect); 
            }

        }
    }
    
    for (int y = 0; y < winH; y += tileSize) {
        for (int x = 0; x < winW; x += tileSize) {
            if ((y < tileSize) || (y > winH - tileSize * 2)) {      // loop for render "top&bot"bar
                destRect = { x ,y , tileSize, tileSize };
                SDL_RenderCopy(Game::Grenderer, bg_texture, nullptr, &destRect); 
            }
        }
    }

	// render title
	int TITLE_WIDTH, TITLE_HEIGHT;
   	SDL_QueryTexture(title_texture, nullptr, nullptr, &TITLE_WIDTH, &TITLE_HEIGHT);
	desRect_title = { 910, 590, TITLE_WIDTH * 2, TITLE_HEIGHT * 2 };
	SDL_RenderCopy(Game::Grenderer, title_texture, nullptr, &desRect_title);   
}
