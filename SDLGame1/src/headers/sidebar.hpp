#ifndef SIDEBAR_HPP
#define SIDEBAR_HPP

#include "TextureManager.hpp"

class Game;
class Player;

class Sidebar {
public:
	Sidebar();
	~Sidebar();

	void render(int winW, int winH, Player* player);
	void render_score(int score, int Highscore);
	void render_playerhp(Player* player);

private:
	SDL_Texture* bg_texture;
	SDL_Texture* digit_texture;
	SDL_Texture* powerngraze_texture;
	SDL_Texture* title_texture;
	SDL_Texture* hp_texture;
	SDL_Texture* player_texture;

	SDL_Rect destRect;
	SDL_Rect desRect_title;
	SDL_Rect destRect_power;
	SDL_Rect srcRect_digit;
	SDL_Rect destRect_digit;
	SDL_Rect destRect_hp;

	int tileSize;

	const int DIGIT_SIZE = 16;
	const int SCORE_DIGITS = 9;
};

#endif 









