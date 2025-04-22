#ifndef ITEMS_HPP
#define ITEMS_HPP

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "TextureManager.hpp"
#include "SoundManager.hpp"

class Game;
class Player;
class Enemy;
class Boss;

enum class Itemtype {
	POINT,
	POWER_S,
	POWER_L,
	FULLPOWER,
	ONEUP,
	STAR, 
};

class Item {
public:
	Item(double x, double y, Itemtype type);
	~Item();
	void update(Player* player);
	void render();

	static void enemy_drop(Enemy* enemy, std::vector<Item*>& items, Player* player);
	static void boss_drop(Boss* boss, std::vector<Item*>& items);

	int getPoint() const { return item_point; }
	int getX() const { return hitbox.x; }
	int getY() const { return hitbox.y; }
	SDL_Rect getHitbox() const { return hitbox; }
	Itemtype getType() const { return type; }
	

private:
	static inline int randomspawn(int range, int node, int pos) {
		return (pos + (rand() % range) + node);
	}
	SDL_Texture* item_text;
	double xPos, yPos;
	double fallspeed;
	int item_point;
	Itemtype type;
	SDL_Rect srcRect, destRect;
	SDL_Rect hitbox;

	bool TrackPlayer;

	const int ITEM_Y = 25;
	const int ITEM_POWER_S = 306;
	const int ITEM_POINT = 322;
	const int ITEM_POWER_L = 338;
	const int ITEM_FULLPOWER = 370;
	const int ITEM_ONEUP = 386;
	const int ITEM_STAR = 402;
	const int spriteW = 16, spriteH = 16;
};
#endif 
