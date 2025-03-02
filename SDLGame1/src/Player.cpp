#include "headers/Player.hpp"
#include "headers/Bullets.hpp"
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

void Player::handleInput(const Uint8* keys) {
    dx = dy = 0;

    if (keys[SDL_SCANCODE_LSHIFT]) speed = focusSpeed;
    else speed = baseSpeed;

    if (keys[SDL_SCANCODE_W]) dy = -speed;
    if (keys[SDL_SCANCODE_S]) dy = speed;
    if (keys[SDL_SCANCODE_A]) dx = -speed;
    if (keys[SDL_SCANCODE_D]) dx = speed;
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

void Player::playerShoot(std::vector<Bullet*>& bullets) {
    bullets.push_back(new Bullet(renderer, destRect.x - 1, destRect.y + 12, 0, -20, Bullettype::PLAYER_0)); // shooting function
}

