#include "headers/Bullets.hpp"
#include <iostream>

Bullet::Bullet(SDL_Renderer* renderer, int x, int y, int velx, int vely, Bullettype type)
	: renderer(renderer), vx(velx), vy(vely), type(type) {
	playerbullet_text = TextureManager::LoadTexture("res/player/Reimu_sprite.png", renderer); //will introduce other type later
	srcRect = { 132, 0, 16, 16 }; // sprite size
	destRect = { x, y, 16 * 2, 16 * 2 }; // display size
}

Bullet::~Bullet() {
	SDL_DestroyTexture(playerbullet_text);
}

void Bullet::update() {
	if (type == Bullettype::PLAYER_0) {
		destRect.y += vy;
	}
}

void Bullet::render() {
	SDL_RenderCopy(renderer, playerbullet_text, &srcRect, &destRect);
}

int Bullet::getY() const {
    return destRect.y;
}