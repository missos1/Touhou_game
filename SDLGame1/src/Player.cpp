#include "headers/Player.hpp"
#include "headers/Bullets.hpp"
#include <iostream>
#include <unordered_set>

Player::Player(SDL_Renderer* renderer, int x, int y)
    : renderer(renderer), dx(0), dy(0), speed(9), isFocusing(false), isMovingright(false),
    isFlipped(false) {

    texture = TextureManager::LoadTexture("res/player/idleanimation.png", renderer);
    amulet_text = TextureManager::LoadTexture("res/player/Reimu_sprite.png", renderer);
    rightTexture = TextureManager::LoadTexture("res/player/left.png", renderer);

    totalFrames = 4;
    Ani_speed = 0.1f;
    frameTime = 0.0f;
    currentFrame = 0;
    currentFrameIdle = 0;

    srcRect = { 0, 0, PLAYER_WIDTH, PLAYER_HEIGHT }; // sprite size
    destRect = { x, y, PLAYER_WIDTH * 2, PLAYER_HEIGHT * 2 }; // display size (scaled up)

    srcRect_amu_0 = { 134, 17, 14, 16 };
    srcRect_amu_1 = srcRect_amu_0;
    destRect_amu_0 = { x, y, 14 * 2, 16 * 2 };
    destRect_amu_1 = destRect_amu_0;
}

Player::~Player() {
    SDL_DestroyTexture(texture);
}

void Player::handleInput(const Uint8* keys) {
    dx = dy = 0;

    if (keys[SDL_SCANCODE_LSHIFT]) {
        speed = focusSpeed;
        isFocusing = true;
    }
    else {
        speed = baseSpeed;
        isFocusing = false;
    }
    if (keys[SDL_SCANCODE_W]) dy = -speed;
    if (keys[SDL_SCANCODE_S]) dy = speed;
    if (keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_D]) {
        if (keys[SDL_SCANCODE_A]) {
            dx = -speed;
            isMovingright = true;
            isFlipped = false;
            isIdle = false;
        }
        if (keys[SDL_SCANCODE_D]) {
            dx = speed;
            isMovingright = true;
            isFlipped = true;
            isIdle = false;
        }
    }
    else {
        isIdle = true;
    }
}

void Player::update() {
    frameTime += Ani_speed;

    if (isMovingright) {
        if (currentFrame < 6 && !isIdle) {
            if (frameTime >= 0.5f) {
                frameTime = 0.0f;
                currentFrame++;
                // left right animation     
                srcRect.x = currentFrame * PLAYER_WIDTH;
            }
        }
        else if (currentFrame > 0 && isIdle) {
            isMovingright = true;
            if (frameTime >= 0.01f) {
                frameTime = 0.0f;
                currentFrame--;
                srcRect.x = currentFrame * PLAYER_WIDTH;
            }
        }
        else if (currentFrame == 0 && isIdle) {
            isMovingright = false;
        }
    }
    else {
        if (frameTime >= 1.0f && currentFrame == 0 && isIdle) {
            frameTime = 0.0f;
            currentFrameIdle = (currentFrameIdle + 1) % totalFrames;
            srcRect.x = currentFrameIdle * PLAYER_WIDTH;
        }
    }

    destRect.x += dx;
    destRect.y += dy;

    if (isFocusing) {
        destRect_amu_0.x = destRect.x - 5;
        destRect_amu_0.y = destRect.y - 20;
        destRect_amu_1.x = destRect.x + 24;
        destRect_amu_1.y = destRect_amu_0.y;    // focus mode config
    }
    else {
        destRect_amu_0.x = destRect.x - 45;
        destRect_amu_1.x = destRect.x + PLAYER_WIDTH + 32;
        destRect_amu_0.y = destRect.y + 24;
        destRect_amu_1.y = destRect_amu_0.y;
    }
}

void Player::render() {

    if (isMovingright) {
        SDL_RenderCopyEx(renderer, rightTexture, &srcRect, &destRect, 0, nullptr, isFlipped ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
    }
    else {
        SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
    }

    if (powerlv >= 3) {
        
        SDL_RenderCopyEx(renderer, amulet_text, &srcRect_amu_0, &destRect_amu_0, 0, nullptr, SDL_FLIP_NONE);
        SDL_RenderCopyEx(renderer, amulet_text, &srcRect_amu_1, &destRect_amu_1, 0, nullptr, SDL_FLIP_NONE); // amulet
    }
}

void Player::playerShoot(std::vector<Bullet*>& bullets) {
    // powerlv manager
    double bulletspeed = -60.0;
    std::vector<int> angle;
    switch (powerlv) {
        case 1:
            angle = { 0 };
            break;
        case 2:
            angle = { 2, -2 };
            break;
        case 3:
            angle = { 7, 0, -7 };
            bullets.push_back(new Bullet(renderer, destRect_amu_0.x + 3, destRect_amu_0.y, 0, bulletspeed, Bullettype::PLAYER_1));
            bullets.push_back(new Bullet(renderer, destRect_amu_1.x + 3, destRect_amu_1.y, 0, bulletspeed, Bullettype::PLAYER_1));
            break;
        case 4:
            angle = { 9, 4, -4, -9 };
            bullets.push_back(new Bullet(renderer, destRect_amu_0.x + 20, destRect_amu_0.y + 24, 0, bulletspeed, Bullettype::PLAYER_1));
            bullets.push_back(new Bullet(renderer, destRect_amu_0.x - 16, destRect_amu_0.y + 24, 0, bulletspeed, Bullettype::PLAYER_1));
            bullets.push_back(new Bullet(renderer, destRect_amu_1.x + 20, destRect_amu_1.y + 24, 0, bulletspeed, Bullettype::PLAYER_1));
            bullets.push_back(new Bullet(renderer, destRect_amu_1.x - 16, destRect_amu_1.y + 24, 0, bulletspeed, Bullettype::PLAYER_1));
            break;
        case 5:
            angle = { 14, 7, 0, -7, -14 };
            bullets.push_back(new Bullet(renderer, destRect_amu_0.x + 20, destRect_amu_0.y + 24, 0, bulletspeed, Bullettype::PLAYER_1));
            bullets.push_back(new Bullet(renderer, destRect_amu_0.x - 16, destRect_amu_0.y + 24, 0, bulletspeed, Bullettype::PLAYER_1));
            bullets.push_back(new Bullet(renderer, destRect_amu_1.x + 20, destRect_amu_1.y + 24, 0, bulletspeed, Bullettype::PLAYER_1));
            bullets.push_back(new Bullet(renderer, destRect_amu_1.x - 16, destRect_amu_1.y + 24, 0, bulletspeed, Bullettype::PLAYER_1));
            bullets.push_back(new Bullet(renderer, destRect_amu_0.x + 3, destRect_amu_0.y, 0, bulletspeed, Bullettype::PLAYER_1));
            bullets.push_back(new Bullet(renderer, destRect_amu_1.x + 3, destRect_amu_1.y, 0, bulletspeed, Bullettype::PLAYER_1));
            break;
        case 6:
            bullets.push_back(new Bullet(renderer, destRect_amu_0.x + 20, destRect_amu_0.y + 24, bulletspeed, bulletspeed, Bullettype::PLAYER_1));
    }
    
    for (int angle : angle) {
        double vx = angle;
        bullets.push_back(new Bullet(renderer, destRect.x - 1, destRect.y + 80, vx, bulletspeed, Bullettype::PLAYER_0));
    }
}

int Player::getY() {
    return destRect.y;
}

int Player::getX() {
    return destRect.x;
}

