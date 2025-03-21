#ifndef MENU_HPP
#define MENU_HPP

#include <SDL.h>>
#include <SDL_image.h>

class Game;
class TextureManager;

class Menu {
public:
	Menu();
	~Menu();

	void handleInput(const Uint8* keys);
	void render();

private:
	SDL_Rect bg, Quit, Start;
	SDL_Texture* background;
	SDL_Texture* QuitSL;
	SDL_Texture* QuitUSL;
	SDL_Texture* StartSL;
	SDL_Texture* StartUSL;

	int selectedOption;
};

#endif