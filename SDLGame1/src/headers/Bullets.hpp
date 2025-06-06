#ifndef BULLETS_HPP
#define BULLETS_HPP

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "TextureManager.hpp"


class Game;

enum class Bullettype { // types of bullets
    PLAYER_0,
    PLAYER_1,
    ENEMY_KUNAI_RD,
    ENEMY_KUNAI_BL,
    ENEMY_KUNAI_GR,
    ENEMY_KNIFE,
    ENEMY_RICE_RD,
    ENEMY_RICE_BL,
    ENEMY_RICE_GR,
    ENEMY_ROUND1,
	ENEMY_PEARL_BL,
	ENEMY_PEARL_RD,
};

class Bullet {
public:
    Bullet(double x, double y, double velx, double vely, Bullettype type, int Ricochet);
    ~Bullet();

    void update();
    void render();

    int getX() const { return hitbox.x; }
    int getY() const { return hitbox.y; }
    SDL_Rect getHitbox() const { return hitbox; }
    void GrazeUpdate() { grazed = true; }
    bool getGrazeState() const { return grazed; }
    int getDmg() const { return dmg; }

private:
    void RicochetHandling();

    SDL_Texture* playerbullet_text;
    SDL_Texture* enemybullet_text;
    SDL_Rect srcRect, destRect;
    SDL_Rect hitbox;

    int Ricochet;
    int dmg;

    bool grazed = false;

    double xPos, yPos;  // Store actual position as double
    double vx, vy;
    double savedVx, savedVy;
    Bullettype type;

    const int S_HITBOX_SIZE = 8;
    const int M_HITBOX_SIZE = 12;
    const int L_HITBOX_SIZE = 16;
    const int XL_HITBOX_SIZE = 32;

    const int ROUND0_Y = 57;
    const int ROUND1_Y = 73;
    const int RICE_Y = 89;
    const int KUNAI_Y = 105;
    const int KNIFE_Y = 185;
    const int DEFAULT_X = 306;

    const int PEARL_X = 587;
    const int PEARL_Y = 25;

	const int XL_RED = 0;
	const int XL_BLUE = 64;
	const int XL_GREEN = 128;
	const int XL_YELLOW = 192;

    const int L_RED = 32;
    const int L_PINK = 64;
    const int L_BLUE = 96;
    const int L_CYAN = 128;
    const int L_GREEN = 160;
    const int L_YELLOW = 192;
    const int L_WHITE = 224;
};

#endif