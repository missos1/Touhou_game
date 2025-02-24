#include "sidebar.hpp"

Sidebar::Sidebar(SDL_Renderer* renderer) : renderer(renderer) {

	bgtexture_0 = TextureManager::LoadTexture("res/BGTEXTURE.png", renderer);
	
}
Sidebar::~Sidebar() {
	SDL_DestroyTexture(bgtexture_0);
}

void Sidebar::render(int winW, int winH) {
    int w, h;
    SDL_QueryTexture(bgtexture_0, nullptr, nullptr, &w, &h);
    tileSize = w;

    for (int y = 0; y < winH; y += tileSize) {
        for (int x = 0; x < winW; x += tileSize) {
            if ((x < tileSize * 2) || (x > winW - tileSize * 14)) {
                destRect = { x ,y , tileSize, tileSize };
                SDL_RenderCopy(renderer, bgtexture_0, nullptr, &destRect); //render background
            }

        }
    }

    for (int y = 0; y < winH; y += tileSize) {
        for (int x = 0; x < winW; x += tileSize) {
            if ((y < tileSize) || (y > winH - tileSize * 2)) {
                destRect = { x ,y , tileSize, tileSize };
                SDL_RenderCopy(renderer, bgtexture_0, nullptr, &destRect); //render background
            }
        }
    }
}