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
	SDL_Texture* bgtexture_0;
	SDL_Rect destRect;
	int tileSize;

};

#endif 









