#ifndef BULLETS_HPP
#define BULLETS_HPP

#include <SDL.h>
#include <SDL_image.h>
#include "TextureManager.hpp"

enum class Bullettype {
	PLAYER_0,
	PLAYER_1,
	ENEMY_0,	// types of bullet
	ENEMY_1,
	ENEMY_2,
	ENEMY_3,
};


class Bullet {
public:
	Bullet(SDL_Renderer* renderer, int x, int y, int velx, int vely, Bullettype type);
	~Bullet();
	void update();
	void render();
	int getY() const;

private:
	SDL_Renderer* renderer;
	SDL_Texture* playerbullet_text;
	SDL_Rect srcRect, destRect;

	int vx, vy;
	int savedVx, savedVy;
	Bullettype type;
};

#endif