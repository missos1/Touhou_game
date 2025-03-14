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
		hitbox = destRect;
		break;
	case Bullettype::PLAYER_1:
		playerbullet_text = TextureManager::LoadTexture("res/player/Reimu_sprite.png"); //will introduce other type later;
		spriteXpos = 200; spriteYpos = 0; spriteW = 9; spriteH = 48;
		srcRect = { spriteXpos, spriteYpos, spriteW, spriteH };
		destRect = { (int)x, (int)y, spriteW * 2, spriteH * 2 };
		hitbox = destRect;
		break;
	case Bullettype::ENEMY_KUNAI:
		enemybullet_text = playerbullet_text = TextureManager::LoadTexture("res/bullets.png");
		spriteXpos = 338; spriteYpos = 105; spriteW = 16; spriteH = 16;
		srcRect = { spriteXpos, spriteYpos, spriteW, spriteH };
		destRect = { (int)x, (int)y, spriteW * 2, spriteH * 2 };
		hitbox = { (int)y, (int)x, 8 , 8};
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
	switch (type) {
	case Bullettype::ENEMY_KUNAI:
		hitbox.x = destRect.x + 12;
		hitbox.y = destRect.y + 12;
		break;
	default:
		hitbox.x = destRect.x;
		hitbox.y = destRect.y;
	}

}

void Bullet::render() {
	static double plyb_angle = 0;
	plyb_angle += 30;
	if (plyb_angle >= 360) plyb_angle = 0;
	switch (type) {
		case Bullettype::PLAYER_0:
			SDL_SetTextureAlphaMod(playerbullet_text, 100);
			SDL_RenderCopyEx(Game::Grenderer, playerbullet_text, &srcRect, &destRect, plyb_angle, nullptr, SDL_FLIP_NONE);
			break;
		case Bullettype::PLAYER_1:
			SDL_SetTextureAlphaMod(playerbullet_text, 100);
			SDL_RenderCopy(Game::Grenderer, playerbullet_text, &srcRect, &destRect);
			break;
		case Bullettype::ENEMY_KUNAI:
			double angle = atan2(vy, vx) * 180 / M_PI;
			SDL_RenderCopyEx(Game::Grenderer, enemybullet_text, &srcRect, &destRect, angle - 90, nullptr , SDL_FLIP_NONE);
			break;
	}
	//SDL_SetRenderDrawColor(Game::Grenderer, 0, 255, 0, 255);
	//SDL_RenderFillRect(Game::Grenderer, &hitbox);  // debug hitboxes
}

int Bullet::getX() const {
	return hitbox.x;
}

int Bullet::getY() const {
    return hitbox.y;
}
