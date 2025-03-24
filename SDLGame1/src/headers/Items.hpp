#ifndef ITEMS_HPP
#define ITEMS_HPP

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "TextureManager.hpp"

class Game;
enum class Itemtype {
	POINT,
	POWER_S,
	POWER_L,
	FULLPOWER,
	ONEUP,
};
class Item {
public:
	Item(double x, double y, Itemtype type);
	~Item();
	void update();
	void render();

	int getY() const;
	int getX() const;
	SDL_Rect getHitbox() const;
	int getPoint() const;
	Itemtype getType() const;

private:
	SDL_Texture* item_text;
	double xPos, yPos;
	double fallspeed;
	int item_point;
	Itemtype type;
	SDL_Rect srcRect, destRect;
	SDL_Rect hitbox;

	const int ITEM_Y = 25;
	const int ITEM_POWER_S = 306;
	const int ITEM_POINT = 322;
	const int ITEM_POWER_L = 338;
	const int ITEM_FULLPOWER = 370;
	const int ITEM_ONEUP = 386;
	const int spriteW = 16, spriteH = 16;
};
#endif 
