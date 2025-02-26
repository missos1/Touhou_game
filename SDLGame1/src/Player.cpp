#include "headers/Player.hpp"
#include <iostream>
#include <unordered_set>

Player::Player(SDL_Renderer* renderer, int x, int y)
    : renderer(renderer), dx(0), dy(0), speed(9), holdTime(0.0f) {

    texture = TextureManager::LoadTexture("res/player/Reimu_sprite.png", renderer);

    totalFrames = 4;
    Ani_speed = 0.1f;
    frameTime = 0.0;
    currentFrame = 0;

    srcRect = { 0, 0, PLAYER_WIDTH, PLAYER_HEIGHT }; // sprite size
    destRect = { x, y, PLAYER_WIDTH * 2, PLAYER_HEIGHT * 2 }; // display size (scaled up)
}

Player::~Player() {
    SDL_DestroyTexture(texture);
}

void Player::handleInput(SDL_Event& event) {
    static std::unordered_set<SDL_Scancode> heldKeys; // smooth movement

    if (event.type == SDL_KEYDOWN) {
		SDL_Scancode scancode = event.key.keysym.scancode;
		heldKeys.insert(scancode);
        // shift to slow
        if (event.key.keysym.scancode == SDL_SCANCODE_LSHIFT || event.key.keysym.sym == SDL_SCANCODE_RSHIFT) {
            isFocusing = true;  
            speed = focusSpeed;
        }
    }

    else if (event.type == SDL_KEYUP) {
        SDL_Scancode scancode = event.key.keysym.scancode;
		heldKeys.erase(scancode);       
        // return to normal speed
        if (event.key.keysym.scancode == SDL_SCANCODE_LSHIFT || event.key.keysym.sym == SDL_SCANCODE_RSHIFT) {
            isFocusing = false; 
            speed = baseSpeed;
        }
    }

    dx = dy = 0;

    if (heldKeys.count(SDL_SCANCODE_W)) dy = -speed; // move up
    if (heldKeys.count(SDL_SCANCODE_S)) dy = speed;// move down
    if (heldKeys.count(SDL_SCANCODE_D)) {
        dx = speed;// move right
        isMoving = true;
    }

    if (heldKeys.count(SDL_SCANCODE_A)) {
        dx = -speed; // move left
        isMoving = true;
    }
    else if (!heldKeys.count(SDL_SCANCODE_A) && !heldKeys.count(SDL_SCANCODE_D)) isMoving = false;
}

void Player::update() {
    frameTime += Ani_speed;

    //srcRect.y = isMoving ? PLAYER_HEIGHT : 0;

    /*if (isMoving) {
        holdTime += Ani_speed;
        if (holdTime <= 0.4f) {
            if (frameTime >= 1.2f && currentFrame <= 2) {
                frameTime = 0.0f;
                currentFrame++;
            }
        }
        else {
            if (frameTime >= 1.2f && currentFrame <= 3) {
                frameTime = 0.0f;
                currentFrame++;
            }
        }
    }*/

    //else {
        if (frameTime >= 1.0f) {
            frameTime = 0.0f;
            currentFrame = (currentFrame + 1) % totalFrames;
        }
    //}

        srcRect.x = currentFrame * PLAYER_WIDTH;

        destRect.x += dx;
        destRect.y += dy;
     
}

void Player::render() {
    SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
}

