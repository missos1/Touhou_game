#include "Player.h"
#include <iostream>

Player::Player(SDL_Renderer* renderer, int x, int y) : renderer(renderer), speed(1), dx(0), dy(0) {
    texture = TextureManager::LoadTexture("res/player/reimusprite_test.png", renderer);

    srcRect = { 0, 0, 23, 43 }; // Sprite size
    destRect = { x, y, 23, 43 }; // Display size (scaled up)
}

Player::~Player() {
    SDL_DestroyTexture(texture);
}

void Player::handleInput(SDL_Event& event) {
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
        case SDLK_w: dy = -speed; break; // Move up
        case SDLK_s: dy = speed; break;  // Move down
        case SDLK_a: dx = -speed; break; // Move left
        case SDLK_d: dx = speed; break;  // Move right
        }
    }
    if (event.type == SDL_KEYUP) {
        switch (event.key.keysym.sym) {
        case SDLK_w:
        case SDLK_s: dy = 0; break;
        case SDLK_a:
        case SDLK_d: dx = 0; break;
        }
    }
}

void Player::update() {
    destRect.x += dx;
    destRect.y += dy;
}

void Player::render() {
    SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
}