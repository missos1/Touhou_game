#include "headers/Bullets.hpp"
#include "headers/Game.hpp"
#include <iostream>
#include <cmath>

Bullet::Bullet(double x, double y, double velx, double vely, Bullettype type)
	: xPos(x), yPos(y), vx(velx), vy(vely), type(type),
	destRect{ (int)x, (int)y, 0, 0 }, srcRect{ 0, 0, 0, 0 },
	playerbullet_text(nullptr), enemybullet_text(nullptr),
	savedVx(0), savedVy(0) {

	int spriteW = 0, spriteH = 0, spriteXpos = 0, spriteYpos = 0;
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
	case Bullettype::ENEMY_KUNAI:
		enemybullet_text = playerbullet_text = TextureManager::LoadTexture("res/bullets.png");
		spriteXpos = 338; spriteYpos = 105; spriteW = 16; spriteH = 16;
		srcRect = { spriteXpos, spriteYpos, spriteW, spriteH };
		destRect = { (int)x, (int)y, spriteW * 2, spriteH * 2 };
		break;
	}
}


Bullet::~Bullet() {
	SDL_DestroyTexture(playerbullet_text);
}

void Bullet::update() {
	xPos += vx;
	yPos += vy;
	destRect.x = static_cast<int>(xPos);
	destRect.y = static_cast<int>(yPos);
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
		case Bullettype::ENEMY_KUNAI:
			double angle = atan2(vy, vx) * 180 / M_PI;
			SDL_RenderCopyEx(Game::Grenderer, enemybullet_text, &srcRect, &destRect, angle - 90, nullptr , SDL_FLIP_NONE);
			break;
	}
}

int Bullet::getY() const {
    return destRect.y;
}