#include "headers/Bullets.hpp"
#include "headers/Game.hpp"
#include <iostream>
#include <cmath>

Bullet::Bullet(double x, double y, double velx, double vely, Bullettype type)
	: vx(velx), vy(vely), type(type), destRect{ 0, 0, 0, 0 }, srcRect{ 0, 0, 0, 0 },
	playerbullet_text(nullptr), savedVx(0), savedVy(0){
	switch (type) {
		case Bullettype::PLAYER_0:
			playerbullet_text = TextureManager::LoadTexture("res/player/Reimu_sprite.png"); //will introduce other type later;
			srcRect = { 132, 0, 16, 16 }; // sprite size
			destRect = { (int) x, (int) y, 16 * 3, 16 * 3 }; // display size
			break;
		case Bullettype::PLAYER_1:
			playerbullet_text = TextureManager::LoadTexture("res/player/Reimu_sprite.png"); //will introduce other type later;
			srcRect = { 200, 0, 9, 48 }; // sprite size
			destRect = { (int) x, (int) y, 9 * 2, 48 * 2 }; // display size
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
			destRect.x += vx;
			destRect.y += vy;
			break;
		case Bullettype::ENEMY_0:
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