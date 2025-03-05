#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SDL.h>
#include <vector>
#include "TextureManager.hpp"

class Bullet;

class Player {
public:
    Player(SDL_Renderer* renderer, int x, int y);
    ~Player();

    void handleInput(const Uint8* keys);
    void update();
    void render();
    void playerShoot(std::vector<Bullet*>& bullets);
    int getY();
    int getX();
private:
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    SDL_Rect srcRect, destRect;
    SDL_Texture* amulet_text;
    SDL_Rect srcRect_amu_0, destRect_amu_0;
    SDL_Rect srcRect_amu_1, destRect_amu_1;

    int baseSpeed = 9;   
    int focusSpeed = 4; 
    int speed;
    int dx, dy;  // movement deltas
    int powerlv = 6;
    bool isFocusing;
    /*bool isFocusing = false;  
    bool isIdle = true;*/

    const int PLAYER_HEIGHT = 50;
    const int PLAYER_WIDTH = 32;

    int currentFrame;
    int totalFrames;
    float Ani_speed;
    float frameTime;
    //bool isMoving;
    //bool isShooting;
    //bool reverseAnimation;
    //float holdTime;
};

#endif
