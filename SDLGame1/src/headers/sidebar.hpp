#ifndef SIDEBAR_HPP
#define SIDEBAR_HPP

#include "TextureManager.hpp"
#include <SDL_ttf.h>

class Game;
class Player;
class SoundManager;

class Sidebar {
public:
	Sidebar();
	~Sidebar();

	void render(int winW, int winH, Player* player);
	void render_background();
	void handleInputs_pausescreen(const SDL_Event& event);

private:
	void render_score(int score, int Highscore);
	void render_playerhp(Player* player);
	void render_pausescreen();

	SDL_Texture* frame_texture;
	SDL_Texture* powerngraze_texture;
	SDL_Texture* title_texture;
	SDL_Texture* hp_texture;
	SDL_Texture* player_texture;
	SDL_Texture* background_texture;

	SDL_Texture* text_texture;

	SDL_Rect destRect;
	SDL_Rect destRect_hp;

	SDL_Color white = { 255, 255, 255 };
	SDL_Color red = { 255, 0, 0 };
	SDL_Color light_gray = { 200, 200, 200 };
	SDL_Color gray = { 128, 128, 128 };
	SDL_Color dark_gray = { 50, 50, 50 };

	TTF_Font* font0;
	TTF_Font* font1;
	int tileSize;

	const int DIGIT_SIZE = 16;
	const int SCORE_DIGITS = 9;

	int selectedOption;
};

#endif 









