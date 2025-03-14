#ifndef BULLETS_HPP
#define BULLETS_HPP

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "TextureManager.hpp"

class Game;

enum class Bullettype {
	PLAYER_0,
	PLAYER_1,
	ENEMY_KUNAI,	// types of bullets/bullet patterns
	ENEMY_1,
	ENEMY_2,
	ENEMY_3,
};


class Bullet {
public:
    Bullet(double x, double y, double velx, double vely, Bullettype type);
    ~Bullet();
    void update();
    void render();
    int getY() const;
    int getX() const;

private:
    SDL_Texture* playerbullet_text;
    SDL_Texture* enemybullet_text;
    SDL_Rect srcRect, destRect;

    double xPos, yPos;  // Store actual position as double
    double vx, vy;
    double savedVx, savedVy;
    Bullettype type;
};

#endif