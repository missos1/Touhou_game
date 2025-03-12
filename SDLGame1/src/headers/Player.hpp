#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SDL.h>
#include <SDL_mixer.h>
#include <vector>
#include "TextureManager.hpp"
#include "Game.hpp"  

const int PLAY_AREA_X_MIN = 54;
const int PLAY_AREA_X_MAX = 820;
const int PLAY_AREA_Y_MIN = 30;
const int PLAY_AREA_Y_MAX = 855;

class Bullet;

class Player {
public:
    Player(int x, int y);
    ~Player();

    void handleInput(const Uint8* keys);
    void update();
    void render();
    void playerShoot(std::vector<Bullet*>& bullets);

    int getY() const;
    int getX() const;

private:
    SDL_Texture* texture;
    SDL_Rect srcRect, destRect;
    SDL_Texture* amulet_text;
    SDL_Texture* rightTexture;
    SDL_Rect srcRect_amu_0, destRect_amu_0;
    SDL_Rect srcRect_amu_1, destRect_amu_1;

    Mix_Chunk* shootSound;

    int baseSpeed = 9;
    int focusSpeed = 2;
    int speed;
    int dx, dy;
    int powerlv = 5;
    bool isFocusing;
    bool isIdle = true;

    const int PLAYER_HEIGHT = 43;
    const int PLAYER_WIDTH = 32;

    int currentFrame;
    int currentFrameIdle;
    int totalFrames;
    float Ani_speed;
    float frameTime;
    bool isMovingright;
    bool isFlipped;
};

#endif