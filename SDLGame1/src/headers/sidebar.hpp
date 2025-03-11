#ifndef SIDEBAR_HPP
#define SIDEBAR_HPP

#include "TextureManager.hpp"

class Game;

class Sidebar {
public:
	Sidebar();
	~Sidebar();

	void render(int winW, int winH);

private:
	SDL_Texture* bg_texture;
	SDL_Texture* title_texture;

	SDL_Rect destRect;
	SDL_Rect desRect_title;

	int tileSize;

};

#endif 









