#ifndef SIDEBAR_HPP
#define SIDEBAR_HPP

#include "TextureManager.hpp"
#include <SDL_ttf.h>

class Game;
class Player;
class Boss;
class SoundManager;

class Sidebar {
public:
	Sidebar();
	~Sidebar();

	void render(int winW, int winH, Player* player, Boss* boss);
	void render_background();
	void handleInputs_pausescreen(const SDL_Event& event);

private:
	void render_boss_hud(Boss* boss);
	void render_bosshp(Boss* boss);
	void render_boss_name(Boss* boss);
	void render_boss_spellcard_count(Boss* boss);
	void render_boss_time(Boss* boss);
	void render_score(int score, int Highscore);
	void render_playerhp(Player* player);
	void render_pausescreen();

	SDL_Texture* frame_texture;
	SDL_Texture* powerngraze_texture;
	SDL_Texture* title_texture;
	SDL_Texture* boss_HP_texture;
	SDL_Texture* hp_texture;
	SDL_Texture* player_texture;
	SDL_Texture* background_texture;

	SDL_Texture* text_texture;

	SDL_Rect destRect;
	SDL_Rect destRect_hp;
	SDL_Rect destRect_boss_hp;


	int tileSize;

	const int DIGIT_SIZE = 16;
	const int SCORE_DIGITS = 10;
	const int BOSS_HPBAR_W = 550;

	int selectedOption;
};

#endif 









