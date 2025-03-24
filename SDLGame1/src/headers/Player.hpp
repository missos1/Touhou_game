#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SDL.h>
#include <SDL_mixer.h>
#include <vector>
#include "TextureManager.hpp"

class Bullet;
class Game;
class Enemy;

class Player {
public:
    Player(double x, double y);
    ~Player();

    void handleInput(const Uint8* keys);
    void update();
    void render();
    void playerShoot(std::vector<Bullet*>& bullets);
    void updatePlayerpower(double input);
    void updatePlayerhp(int input);
    void updateGraze();
    
    int getGraze() const;
    int getY() const;
    int getX() const;
    int getPlayerhp() const;
    double getPlayerpowerlv() const;
    SDL_Rect getGrazingBox() const;
    SDL_Rect getHitbox() const;

private:
    SDL_Texture* texture;
    SDL_Texture* amulet_text;
    SDL_Texture* rightTexture;
    SDL_Rect srcRect, destRect;
    SDL_Rect srcRect_amu_0, destRect_amu_0;
    SDL_Rect srcRect_amu_1, destRect_amu_1;
    SDL_Rect hitbox;

    Mix_Chunk* shootSound;

    int baseSpeed = 9;
    int focusSpeed = 3;
    double speed;
    double dx, dy;
    int hp = 7;
    int bomb = 3;
    double powerlv = 1.0;
    int graze;
    bool isFocusing;
    bool isIdle = true;
    bool affectedbytimestop;

    const int PLAYER_HEIGHT = 50;
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