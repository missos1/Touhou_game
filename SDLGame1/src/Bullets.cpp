#include "headers/Player.hpp"
#include "headers/Bullets.hpp"
#include <iostream>

Bullets::Bullets(SDL_Renderer* renderer, int x, int y, int velx, int vely)
	: renderer(renderer), vx(velx), vy(vely) {
	playerbullet_text = TextureManager::LoadTexture("res/player/Reimu_sprite", renderer);
	srcRect = { 128, 0, 32, 32 }; // sprite size
	destRect = { x, y, 32 * 2, 32 * 2 }; // display size
}

Bullets::~Bullets() {
	SDL_DestroyTexture(playerbullet_text);
	
}

void Bullets::update() {
	destRect.x += vx;
	destRect.y += vy;
}

void Bullets::render() {
	SDL_RenderCopy(renderer, playerbullet_text, &srcRect, &destRect);
}