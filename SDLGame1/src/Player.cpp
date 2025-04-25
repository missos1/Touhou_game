#include "headers/Player.hpp"
#include "headers/Bullets.hpp"
#include "headers/Game.hpp"
#include "headers/SoundManager.hpp"
#include <iostream>
#include <unordered_set>

Player::Player(double x, double y)
    : dx(0), dy(0), speed(9), isFocusing(false), isMovingright(false),
    isFlipped(false),  state(PlayerState::NORMAL), graze(0), invince_flag(true),
    powerlv(1.0) {

    texture = TextureManager::LoadTexture("res/player/Reimu_sprite(2).png");
    hitbox_texture = TextureManager::LoadTexture("res/player/hitbox.png");

    // initialize animation
    totalFrames = 4;
    Ani_speed = 0.13f;
    frameTime = 0.0f;
    currentFrame = 0; 
    currentFrameIdle = 0;

    srcRect = { 0, 0, PLAYER_WIDTH, PLAYER_HEIGHT }; // sprite size
    destRect = { static_cast<int>(x), static_cast<int>(y), PLAYER_WIDTH * 2, PLAYER_HEIGHT * 2 }; // display size (scaled up)    

    srcRect_amu_0 = { 162, 18, 13, 13 };
    srcRect_amu_1 = srcRect_amu_0;
    destRect_amu_0 = { static_cast<int>(x), static_cast<int>(y), 13 * 2, 13 * 2 };
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
    if (hitbox_texture) {
        SDL_DestroyTexture(hitbox_texture);
        hitbox_texture = nullptr;
    }
}

void Player::handleInput(const Uint8* keys, std::vector<Bullet*>& player_bullets) {
    dx = dy = 0;
    // keyboard movement
    if (keys[SDL_SCANCODE_LSHIFT]) { // hold shift to slow down
        speed = focusSpeed;
        isFocusing = true;
    }
    else if (state == PlayerState::GOT_HIT) {
        speed = 0;
        isFocusing = false;
    }
    else {
        speed = baseSpeed;
        isFocusing = false;
    }

    if (keys[SDL_SCANCODE_W]) dy = -speed;
    if (keys[SDL_SCANCODE_S]) dy = speed;

    if (keys[SDL_SCANCODE_A]) dx = -speed;
    if (keys[SDL_SCANCODE_D]) dx = speed;
    

    static Uint64 lastShotTime = 0;
    if (keys[SDL_SCANCODE_SPACE] && Game::GamecurrentTime - lastShotTime > 90) { // Handle shooting
        int powerlv = 0;
        playerShoot(player_bullets);
        //player->testshoot(player_bullets);
        lastShotTime = Game::GamecurrentTime;
    }

    if (keys[SDL_SCANCODE_BACKSLASH]) hp++;
}

void Player::update() {
	if (hp >= 8) hp = 8; // limit max hp
	Game::PlayerPowerLV = powerlv;
	Game::PlayerHP = hp;
    animation();
    powerlvhandle();

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

    static Uint64 invincendTime = 0;
    static Uint64 waitTime = 0;

    if (state == PlayerState::GOT_HIT) {
        if (invince_flag) {
            waitTime = Game::GamecurrentTime + 500;
            invince_flag = false;
        }

        if (Game::GamecurrentTime >= waitTime) {
            invincendTime = Game::GamecurrentTime + 3000;
            destRect.x = 410;
            destRect.y = 680;
            state = PlayerState::INVINC;
            static Uint64 waitTime = 0;
            invince_flag = true;
        }
        
    }

    if (state == PlayerState::INVINC) {
        if (Game::GamecurrentTime >= invincendTime) {
            state = PlayerState::NORMAL;
            invincendTime = 0;
        }
    } 

    if (hp <= 0) {
        Game::GameStartTime = 0; // Reset time
        Game::GamePauseTotalTime = 0; // Reset time
        Game::PLAYSCORE = 0; // Reset score
        Game::state = GameState::MENU; // Goes back to menu
        Game::prevState = GameState::PLAYING;
    }
}

void Player::render() {
    if (state == PlayerState::GOT_HIT) {
        SDL_SetTextureAlphaMod(texture, 0);
    } 
    else if (state == PlayerState::INVINC && Game::state != GameState::PAUSE) {
        static int opacity_player = 0;
        static Uint64 dimmedTime = 0;
        if (Game::GamecurrentTime - dimmedTime >= 200) {
            opacity_player += 255;
            if (opacity_player > 255) opacity_player = 0;
            dimmedTime = Game::GamecurrentTime;
        }
        SDL_SetTextureAlphaMod(texture, opacity_player);
    }
    else {
        SDL_SetTextureAlphaMod(texture, 255);
    }

    SDL_RenderCopyEx(Game::Grenderer, texture, &srcRect, &destRect, 0, nullptr, isFlipped ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
    
    static int opacity = 0;

    if (isFocusing) {
        static int angle = 0;
        if (Game::state != GameState::PAUSE) angle = (angle + 1 + 360) % 360;
        
        SDL_SetTextureAlphaMod(hitbox_texture, opacity);
        SDL_RenderCopyEx(Game::Grenderer, hitbox_texture, nullptr, &hitbox_destRect, angle, nullptr, SDL_FLIP_NONE);
        if (opacity >= 255) opacity = 255;
        else opacity += 5;
    }
    else {
        opacity = 0;
    }
    

    if (powerlv >= 3) {
        //std::cout << angle << std::endl;
        static int angle = 0;
        if (Game::state != GameState::PAUSE) angle = (angle + 10 + 360) % 360;

        SDL_RenderCopyEx(Game::Grenderer, texture, &srcRect_amu_0, &destRect_amu_0, angle, nullptr, SDL_FLIP_NONE);
        SDL_RenderCopyEx(Game::Grenderer, texture, &srcRect_amu_1, &destRect_amu_1, -angle, nullptr, SDL_FLIP_NONE); // amulet
    }
    SDL_SetRenderDrawColor(Game::Grenderer, 0, 255, 0, 255); // debug
    SDL_RenderFillRect(Game::Grenderer, &hitbox_ingame);
}

void Player::resetValue() {
    powerlv = 5.00;
    hp = 4;
    graze = 0;
    destRect.x = PLAYER_OG_X;
    destRect.y = PLAYER_OG_Y;
    state = PlayerState::NORMAL;
    invince_flag = true;
}

void Player::playerShoot(std::vector<Bullet*>& bullets) const {
    // powerlv manager
    if (state == PlayerState::GOT_HIT) return;

    double bulletspeed = -50.0;
    std::vector<int> angle;
    std::vector<std::pair<int, int>> position;

    switch ((int) powerlv) {
    case 1:
        angle = { 0 };
        break;
    case 2:
        angle = { 2, -2 };
        break;
    case 3:
        angle = { 4, 0, -4 };
        position = { { 3, -10 } };
        break;
    case 4:
        angle = { 8, 3, -3, -8 };
        position = { { 20, 1 }, { -16, 1} };
        break;
    case 5:
        angle = { 11, 6, 0, -6, -11 };
        position = { { 20, -30}, { -16, -30 }, { 3, -10 } };
        break;
    }

    for (const std::pair<int, int>& pos : position) {
        bullets.emplace_back(new Bullet(destRect_amu_0.x + pos.first, destRect_amu_0.y + pos.second, 0, bulletspeed, Bullettype::PLAYER_1, 0));
        bullets.emplace_back(new Bullet(destRect_amu_1.x + pos.first, destRect_amu_1.y + pos.second, 0, bulletspeed, Bullettype::PLAYER_1, 0));
    }
    
    for (const int& angle : angle) {
        double vx = angle;
        bullets.emplace_back(new Bullet(hitbox_ingame.x - 15, hitbox_ingame.y, vx, bulletspeed, Bullettype::PLAYER_0, 0));
    }
    SoundManager::PlaySound("plshoot", 0, Game::SE_volume);
}

void Player::updatePlayerhp(int input) {
    if (input < 0) state = PlayerState::GOT_HIT;
    else if (input > 0) SoundManager::PlaySound("plheal", 0, Game::SE_volume);
    hp += input;
}

void Player::updateGraze() {
    SoundManager::PlaySound("graze", 0, Game::SE_volume); // Play graze sound
    graze++;
}

void Player::updatePlayerpower(double input) {
    powerlv += input;
    if (powerlv >= 5.0) powerlv = 5.0;
    else if (powerlv <= 1.0) powerlv = 1.0;
}


// Private functions below

void Player::animation() {
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
        srcRect.y = PLAYER_HEIGHT;
    }
    else {
        if (frameTime >= 1.0f && currentFrame == 0 && isIdle) { // idle animation
            frameTime = 0.0f;
            isFlipped = false;
            currentFrameIdle = (currentFrameIdle + 1) % totalFrames;
            srcRect.y = 0;
            srcRect.x = currentFrameIdle * PLAYER_WIDTH;
        }
    }
}

void Player::powerlvhandle() {
    static int prevpowerlv = 1;
    int pwlv = static_cast<int>(powerlv);
    if (pwlv == prevpowerlv) return;

    if (pwlv > prevpowerlv) SoundManager::PlaySound("pl_powerup", 0, Game::SE_volume);

    prevpowerlv = pwlv;
   
}