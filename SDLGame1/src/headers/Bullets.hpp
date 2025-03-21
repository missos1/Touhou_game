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
    ENEMY_KUNAI_RD,
    ENEMY_KUNAI_BL,
    ENEMY_KNIFE,// types of bullets/bullet patterns
    ENEMY_RICE,
    ENEMY_ROUND1,
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
    int getDmg() const;

    bool getGrazeState() const;
    void GrazeUpdate();
    SDL_Rect getHitbox() const;


private:
    SDL_Texture* playerbullet_text;
    SDL_Texture* enemybullet_text;
    SDL_Rect srcRect, destRect;
    SDL_Rect hitbox;

    int dmg;

    bool grazed = false;

    double xPos, yPos;  // Store actual position as double
    double vx, vy;
    double savedVx, savedVy;
    Bullettype type;

    const int S_HITBOX_SIZE = 8;
    const int M_HITBOX_SIZE = 12;
    const int L_HITBOX_SIZE = 16;

    const int ROUND0_Y = 57;
    const int ROUND1_Y = 73;
    const int RICE_Y = 89;
    const int KUNAI_Y = 105;
    const int KNIFE_Y = 185;
    const int DEFAULT_X = 306;

    //const int S_

    const int L_RED = 32;
    const int L_PINK = 64;
    const int L_BLUE = 96;
    const int L_CYAN = 128;
    const int L_GREEN = 160;
    const int L_YELLOW = 192;
    const int L_WHITE = 224;
};

#endif