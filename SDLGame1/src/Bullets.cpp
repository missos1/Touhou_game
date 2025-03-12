#include "headers/Bullets.hpp"
#include "headers/Game.hpp"
#include <iostream>
#include <cmath>

Bullet::Bullet(double x, double y, double velx, double vely, Bullettype type)
	: vx(velx), vy(vely), type(type), destRect{ 0, 0, 0, 0 }, srcRect{ 0, 0, 0, 0 },
	playerbullet_text(nullptr), savedVx(0), savedVy(0){

	int spriteW = 0, spriteH = 0, spriteXpos= 0, spriteYpos = 0;
	switch (type) {
		case Bullettype::PLAYER_0:
			playerbullet_text = TextureManager::LoadTexture("res/player/Reimu_sprite.png"); //will introduce other type later;
			spriteXpos = 132; spriteYpos = 0; spriteW = 16; spriteH = 16;
			srcRect = { spriteXpos, spriteYpos, spriteW, spriteH };
			destRect = { (int)x, (int)y, spriteW * 3, spriteH * 3 };
			break;
		case Bullettype::PLAYER_1:
			playerbullet_text = TextureManager::LoadTexture("res/player/Reimu_sprite.png"); //will introduce other type later;
			spriteXpos = 200; spriteYpos = 0; spriteW = 9; spriteH = 48;
			srcRect = { spriteXpos, spriteYpos, spriteW, spriteH };
			destRect = { (int)x, (int)y, spriteW * 2, spriteH * 2 };
			break;
		case Bullettype::ENEMY_0:
			break;
	}
	
}

Bullet::~Bullet() {
	SDL_DestroyTexture(playerbullet_text);
}

void Bullet::update() {
	switch (type) {
		case Bullettype::PLAYER_0:
		case Bullettype::PLAYER_1:
		case Bullettype::ENEMY_0:
			destRect.x += vx;
			destRect.y += vy;
			break;
	}
}

void Bullet::render() {
	SDL_SetTextureAlphaMod(playerbullet_text, 100);
	switch (type) {
		case Bullettype::PLAYER_0:
			SDL_RenderCopyEx(Game::Grenderer, playerbullet_text, &srcRect, &destRect, 35, nullptr, SDL_FLIP_NONE);
			break;
		case Bullettype::PLAYER_1:
			SDL_RenderCopy(Game::Grenderer, playerbullet_text, &srcRect, &destRect);
			break;
	}
}

int Bullet::getY() const {
    return destRect.y;
}