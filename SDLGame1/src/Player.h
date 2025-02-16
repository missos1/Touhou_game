#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include <SDL.h>
#include "TextureManager.h"

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

    int speed;
    int dx, dy;  // Movement deltas
};

#endif
