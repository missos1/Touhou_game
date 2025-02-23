#include "Player.hpp"
#include <iostream>

Player::Player(SDL_Renderer* renderer, int x, int y)
    : renderer(renderer), dx(0), dy(0), speed(9) {

    texture = TextureManager::LoadTexture("res/player/Reimu_sprite.png", renderer);

    totalFrames = 4;
    Ani_speed = 0.1f;
    frameTime = 0.0;
    currentFrame = 0;

    srcRect = { 0, 0, PLAYER_WIDTH, PLAYER_HEIGHT }; // Sprite size
    destRect = { x, y, PLAYER_WIDTH * 2, PLAYER_HEIGHT * 2 }; // Display size (scaled up)
}

Player::~Player() {
    SDL_DestroyTexture(texture);
}

void Player::handleInput(SDL_Event& event) {
    if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_LSHIFT || event.key.keysym.sym == SDLK_RSHIFT) {
            isFocusing = true;  // shift to slow
            speed = focusSpeed;
        }

        switch (event.key.keysym.sym) {
        case SDLK_w: dy = -speed; break; // Move up
        case SDLK_s: dy = speed; break;  // Move down
        case SDLK_a: dx = -speed; break; // Move left
        case SDLK_d: dx = speed; break;  // Move right
        }
    }

    if (event.type == SDL_KEYUP) {
        if (event.key.keysym.sym == SDLK_LSHIFT || event.key.keysym.sym == SDLK_RSHIFT) {
            isFocusing = false; // return to normal speed
            speed = baseSpeed;
        }

        switch (event.key.keysym.sym) {
        case SDLK_w:
        case SDLK_s: dy = 0; break;
        case SDLK_a:
        case SDLK_d: dx = 0; break;
        }
    }
}

void Player::update() {
    frameTime += Ani_speed;

    if (frameTime >= 1.0f) {
        frameTime = 0.0f;
        currentFrame = (currentFrame + 1) % totalFrames;
    }

    srcRect.x = currentFrame * PLAYER_WIDTH;

    destRect.x += dx;
    destRect.y += dy;
}

void Player::render() {
    SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
}
