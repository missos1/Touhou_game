#ifndef SIDEBAR_H
#define SIDEBAR_H

#include "TextureManager.hpp"

class Sidebar {
public:
	Sidebar(SDL_Renderer* renderer);
	~Sidebar();

	void render(int winW, int winH);

private:
	SDL_Renderer* renderer;
	SDL_Texture* bg_texture;
	SDL_Texture* title_texture;

	SDL_Rect destRect;
	SDL_Rect desRect_title;

	int tileSize;

};

#endif 









