#include "headers/Player.hpp"
#include "headers/Bullets.hpp"
#include "headers/Game.hpp"
#include "headers/SoundManager.hpp"
#include <iostream>
#include <unordered_set>

Player::Player(double x, double y)
    : dx(0), dy(0), speed(9), isFocusing(false), isMovingright(false),
    isFlipped(false),  affectedbytimestop(false), graze(0) {

    texture = TextureManager::LoadTexture("res/player/idleanimation.png");
    amulet_text = TextureManager::LoadTexture("res/player/Reimu_sprite.png");
    rightTexture = TextureManager::LoadTexture("res/player/rightAni.png");
    hitbox_texture = TextureManager::LoadTexture("res/player/hitbox.png");

    // initialize animation
    totalFrames = 4;
    Ani_speed = 0.13f;
    frameTime = 0.0f;
    currentFrame = 0; 
    currentFrameIdle = 0;

    srcRect = { 0, 0, PLAYER_WIDTH, PLAYER_HEIGHT }; // sprite size
    destRect = { static_cast<int>(x), static_cast<int>(y), PLAYER_WIDTH * 2, PLAYER_HEIGHT * 2 }; // display size (scaled up)    

    srcRect_amu_0 = { 134, 17, 14, 16 };
    srcRect_amu_1 = srcRect_amu_0;
    destRect_amu_0 = { static_cast<int>(x), static_cast<int>(y), 14 * 2, 16 * 2 };
    destRect_amu_1 = destRect_amu_0;

    hitbox_ingame = { 0, 0, PLAYER_HB_SIZE, PLAYER_HB_SIZE };
    hitbox_destRect = { 0, 0, 64 * 2 , 64 * 2 };
}

Player::~Player() {
    // Free textures
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
    if (amulet_text) {
        SDL_DestroyTexture(amulet_text);
        amulet_text = nullptr;
    }
    if (rightTexture) {
        SDL_DestroyTexture(rightTexture);
        rightTexture = nullptr;
    }
    if (hitbox_texture) {
        SDL_DestroyTexture(hitbox_texture);
        hitbox_texture = nullptr;
    }
}

void Player::handleInput(const Uint8* keys) {
    dx = dy = 0;
    // keyboard movement
    if (keys[SDL_SCANCODE_LSHIFT]) { // hold shift to slow down
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
        if (keys[SDL_SCANCODE_A]) dx = -speed;
        if (keys[SDL_SCANCODE_D]) dx = speed;
    } 
}

void Player::update() {
    if ((int)dx != 0) {
        if (dx > 0) isFlipped = true; // flags for animations
        else isFlipped = false;

        isIdle = false;
        isMovingright = true;
    }
    else if ((int)dx == 0) {
        isIdle = true;
    }

    frameTime += Ani_speed;
    // not gonna lie this is pain to figure out
    if (isMovingright) {
        if (currentFrame <= 7 && !isIdle) { // left right animation  
            if (frameTime >= 0.5f && currentFrame < 4) { // if less than 4 then increment to next fram
                frameTime = 0.0f;
                currentFrame++; 
         
            }
            else if (currentFrame >= 4 && frameTime >= 1.0f) { // if more than 4 then looping from 7 to 4
                frameTime = 0.0f;
                ++currentFrame;
                if (currentFrame >= 7) currentFrame = 4;
            }
        }
        else if (currentFrame > 0 && isIdle) { // if key is released then buffers to slowly play frame back
            if (currentFrame > 4) currentFrame = 4;
            if (frameTime >= 0.01f) {
                frameTime = 0.0f;
                currentFrame--;              
            }
        }
        else if (currentFrame == 0 && isIdle) { // after playing animation, return to idle animation
            isMovingright = false;
        }
        srcRect.x = currentFrame * PLAYER_WIDTH;
    }
    else {
        if (frameTime >= 1.0f && currentFrame == 0 && isIdle) { // idle animation
            frameTime = 0.0f;
            currentFrameIdle = (currentFrameIdle + 1) % totalFrames;
            srcRect.x = currentFrameIdle * PLAYER_WIDTH;
        }
    }

    destRect.x += (int) dx;
    destRect.y += (int) dy; 

    if (destRect.x <= PLAY_AREA_X_MIN) destRect.x = PLAY_AREA_X_MIN;
    if (destRect.x >= PLAY_AREA_X_MAX) destRect.x = PLAY_AREA_X_MAX; // limit in fram
    if (destRect.y <= PLAY_AREA_Y_MIN) destRect.y = PLAY_AREA_Y_MIN;
    if (destRect.y >= PLAY_AREA_Y_MAX) destRect.y = PLAY_AREA_Y_MAX;

    hitbox_ingame.x = destRect.x + 28;
    hitbox_ingame.y = destRect.y + 42;
    hitbox_destRect.x = destRect.x - 31;
    hitbox_destRect.y = destRect.y - 16;


    if (isFocusing) {
        destRect_amu_0.x = destRect.x + 2;
        destRect_amu_0.y = destRect.y - 32;
        destRect_amu_1.x = destRect_amu_0.x + 35;
        destRect_amu_1.y = destRect_amu_0.y;    // focus mode config
    }
    else {
        destRect_amu_0.x = destRect.x - 25;
        destRect_amu_1.x = destRect.x + PLAYER_WIDTH + 32;
        destRect_amu_0.y = destRect.y + 24;
        destRect_amu_1.y = destRect_amu_0.y;
    }

    static double prevpowerlv = 1.0;

    if (powerlv != prevpowerlv) {
        //std::cout << "player powerlv: " << powerlv << std::endl;
        if ((powerlv == 2.0 || powerlv == 3.0 || powerlv == 4.0 || powerlv == 5.0) && (powerlv > prevpowerlv)) SoundManager::PlaySound("pl_powerup",0 ,255);
        prevpowerlv = powerlv;
    }
}

void Player::render() {
    if (isMovingright) {
        SDL_RenderCopyEx(Game::Grenderer, rightTexture, &srcRect, &destRect, 0, nullptr, isFlipped ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
    }
    else {
        SDL_RenderCopy(Game::Grenderer, texture, &srcRect, &destRect);
    }
    static int angle = 0;
    angle = (angle + 10 + 360) % 360;

    if (isFocusing) {
        static int angle = 0;
        angle = (angle + 1 + 360) % 360;
        SDL_RenderCopyEx(Game::Grenderer, hitbox_texture, nullptr, &hitbox_destRect, angle, nullptr, SDL_FLIP_NONE);
    }

    if (powerlv >= 3) {
        //std::cout << angle << std::endl;
        static int angle = 0;
        angle = (angle + 10 + 360) % 360;
        SDL_SetTextureAlphaMod(amulet_text, 255);
        SDL_RenderCopyEx(Game::Grenderer, amulet_text, &srcRect_amu_0, &destRect_amu_0, angle, nullptr, SDL_FLIP_NONE);
        SDL_RenderCopyEx(Game::Grenderer, amulet_text, &srcRect_amu_1, &destRect_amu_1, -angle, nullptr, SDL_FLIP_NONE); // amulet
    }
    //SDL_SetRenderDrawColor(Game::Grenderer, 0, 255, 0, 255); // debug
    //SDL_RenderFillRect(Game::Grenderer, &hitbox_ingame);

}

void Player::playerShoot(std::vector<Bullet*>& bullets) const {
    // powerlv manager
    double bulletspeed = -50.0;
    std::vector<int> angle;


    switch ((int) powerlv) {
    case 1:
        angle = { 0 };
        break;
    case 2:
        angle = { 2, -2  };
        break;
    case 3:
        angle = { 4, 0, -4 };
        bullets.emplace_back(new Bullet(destRect_amu_0.x + 3, destRect_amu_0.y, 0, bulletspeed, Bullettype::PLAYER_1));
        bullets.emplace_back(new Bullet(destRect_amu_1.x + 3, destRect_amu_1.y, 0, bulletspeed, Bullettype::PLAYER_1));
        break;
    case 4:
        angle = { 7, 2, -2, -7 };
        bullets.emplace_back(new Bullet(destRect_amu_0.x + 20, destRect_amu_0.y + 1, 0, bulletspeed, Bullettype::PLAYER_1));
        bullets.emplace_back(new Bullet(destRect_amu_0.x - 16, destRect_amu_0.y + 1, 0, bulletspeed, Bullettype::PLAYER_1));
        bullets.emplace_back(new Bullet(destRect_amu_1.x + 20, destRect_amu_1.y + 1, 0, bulletspeed, Bullettype::PLAYER_1));
        bullets.emplace_back(new Bullet(destRect_amu_1.x - 16, destRect_amu_1.y + 1, 0, bulletspeed, Bullettype::PLAYER_1));
        break;
    case 5:
        angle = { 8, 4, 0, -4, -8 };
        bullets.emplace_back(new Bullet(destRect_amu_0.x + 20, destRect_amu_0.y - 30, 0, bulletspeed, Bullettype::PLAYER_1));
        bullets.emplace_back(new Bullet(destRect_amu_0.x - 16, destRect_amu_0.y - 30, 0, bulletspeed, Bullettype::PLAYER_1));
        bullets.emplace_back(new Bullet(destRect_amu_1.x + 20, destRect_amu_1.y - 30, 0, bulletspeed, Bullettype::PLAYER_1));
        bullets.emplace_back(new Bullet(destRect_amu_1.x - 16, destRect_amu_1.y - 30, 0, bulletspeed, Bullettype::PLAYER_1));
        bullets.emplace_back(new Bullet(destRect_amu_0.x + 3, destRect_amu_0.y - 40, 0, bulletspeed, Bullettype::PLAYER_1));
        bullets.emplace_back(new Bullet(destRect_amu_1.x + 3, destRect_amu_1.y - 40, 0, bulletspeed, Bullettype::PLAYER_1));
        break;
    }
    
    for (int angle : angle) {
        double vx = angle;
        bullets.emplace_back(new Bullet(hitbox_ingame.x - 15, hitbox_ingame.y, vx, bulletspeed, Bullettype::PLAYER_0));
    }
    SoundManager::PlaySound("plshoot", 0, 64);
}

void Player::updatePlayerhp(int input) {
    hp += input;
}

void Player::updateGraze() {
    SoundManager::PlaySound("graze", 0, 64); // Play graze sound
    graze++;
}

void Player::updatePlayerpower(double input) {
    powerlv += input;
    if (powerlv >= 5.0) powerlv = 5.0;
    else if (powerlv <= 1.0) powerlv = 1.0;
}