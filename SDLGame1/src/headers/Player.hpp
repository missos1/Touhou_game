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
    void playerShoot(std::vector<Bullet*>& bullets) const;
    void updatePlayerpower(double input);
    void updatePlayerhp(int input);
    void updateGraze();
    
    int getGraze() const { return graze; }
    int getY() const { return hitbox_ingame.y - 20; }
    int getX() const { return hitbox_ingame.x - 10; }
    int getPlayerhp() const { return hp; }
    SDL_Rect getGrazingBox() const { return hitbox_destRect; }
    SDL_Rect getHitbox() const { return hitbox_ingame; }
    double getPlayerpowerlv() const { return powerlv; }

private:
    SDL_Texture* texture;
    SDL_Texture* amulet_text;
    SDL_Texture* rightTexture;
    SDL_Texture* hitbox_texture;
    SDL_Rect hitbox_destRect;
    SDL_Rect srcRect, destRect;
    SDL_Rect srcRect_amu_0, destRect_amu_0;
    SDL_Rect srcRect_amu_1, destRect_amu_1;

    SDL_Rect hitbox_ingame;

    int baseSpeed = 9;
    int focusSpeed = 4;
    double speed;
    double dx, dy;
    int hp = 5;
    double powerlv = 5.0;
    int graze;
    bool isFocusing;
    bool isIdle = true;
    bool affectedbytimestop;

    const int PLAYER_HEIGHT = 48;
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