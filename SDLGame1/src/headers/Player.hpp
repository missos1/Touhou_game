#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SDL.h>
#include "TextureManager.hpp"

class Player {
public:
    Player(SDL_Renderer* renderer, int x, int y);
    ~Player();

    void handleInput(SDL_Event& event);
    void update();
    void render();

private:
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    SDL_Rect srcRect, destRect;

    int baseSpeed = 9;   
    int focusSpeed = 4; 
    int speed;
    int dx, dy;  // movement deltas
    bool isFocusing = false;  
    bool isFiring = false;

    const int PLAYER_HEIGHT = 48;
    const int PLAYER_WIDTH = 32;

    int currentFrame;
    int totalFrames;
    float Ani_speed;
    float frameTime;
};

#endif
