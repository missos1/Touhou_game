#include "headers/Bullets.hpp"
#include "headers/Game.hpp"
#include <iostream>
#include <cmath>

Bullet::Bullet(double x, double y, double velx, double vely, Bullettype type)
	: xPos(x), yPos(y), vx(velx), vy(vely), type(type),
	destRect{ (int)x, (int)y, 0, 0 }, srcRect{ 0, 0, 0, 0 }, // init variables
	playerbullet_text(nullptr), enemybullet_text(nullptr),
	savedVx(0), savedVy(0), hitbox{ 0, 0, 0, 0 }, dmg(0) {

	int spriteW = 0, spriteH = 0, spriteXpos = 0, spriteYpos = 0;
	playerbullet_text = Game::Misc_player_text; // get texture 
	enemybullet_text = Game::enemybullet_text;

	switch (type) {
	case Bullettype::PLAYER_0: // red talisman
		spriteXpos = 132; spriteYpos = 0; spriteW = 16; spriteH = 16;
		srcRect = { spriteXpos, spriteYpos, spriteW, spriteH };
		destRect = { (int)x, (int)y, spriteW * 3, spriteH * 3 };
		hitbox = destRect;
		dmg = 1;
		break;
	case Bullettype::PLAYER_1: // exterminate needles
		spriteXpos = 200; spriteYpos = 0; spriteW = 9; spriteH = 48;
		srcRect = { spriteXpos, spriteYpos, spriteW, spriteH };
		destRect = { (int)x, (int)y, spriteW * 2, spriteH * 2 };
		hitbox = destRect;
		dmg = 2;
		break;
	case Bullettype::ENEMY_KUNAI_RD: // red
		spriteXpos = DEFAULT_X + L_RED; spriteYpos = KUNAI_Y; spriteW = 16; spriteH = 16;
		srcRect = { spriteXpos, spriteYpos, spriteW, spriteH };
		destRect = { (int)x, (int)y, spriteW * 2, spriteH * 2 };
		hitbox = { (int)y, (int)x, S_HITBOX_SIZE , S_HITBOX_SIZE};
		break;
	case Bullettype::ENEMY_KUNAI_BL: // blue 
		spriteXpos = DEFAULT_X + L_BLUE; spriteYpos = KUNAI_Y; spriteW = 16; spriteH = 16;
		srcRect = { spriteXpos, spriteYpos, spriteW, spriteH };
		destRect = { (int)x, (int)y, spriteW * 2, spriteH * 2 };
		hitbox = { (int)y, (int)x, S_HITBOX_SIZE , S_HITBOX_SIZE };
		break;
	case Bullettype::ENEMY_KNIFE: // knife
		spriteXpos = DEFAULT_X + L_BLUE; spriteYpos = KNIFE_Y; spriteW = 32; spriteH = 32;
		srcRect = { spriteXpos, spriteYpos, spriteW, spriteH };
		destRect = { (int)x, (int)y, spriteW * 2, spriteH * 2 };
		hitbox = { (int)y, (int)x, L_HITBOX_SIZE , L_HITBOX_SIZE };
		break;
	case Bullettype::ENEMY_RICE:
		spriteXpos = DEFAULT_X + L_BLUE; spriteYpos = RICE_Y; spriteW = 16; spriteH = 16;
		srcRect = { spriteXpos, spriteYpos, spriteW, spriteH };
		destRect = { (int)x, (int)y, spriteW * 2, spriteH * 2 };
		hitbox = { (int)y, (int)x, S_HITBOX_SIZE , S_HITBOX_SIZE };
		break;
	case Bullettype::ENEMY_ROUND1:
		spriteXpos = DEFAULT_X + L_BLUE; spriteYpos = ROUND1_Y; spriteW = 16; spriteH = 16;
		srcRect = { spriteXpos, spriteYpos, spriteW, spriteH };
		destRect = { (int)x, (int)y, spriteW * 2, spriteH * 2 };
		hitbox = { (int)y, (int)x, M_HITBOX_SIZE , M_HITBOX_SIZE };
		break;
	}
}


Bullet::~Bullet() {

}

void Bullet::update() {
	xPos += vx; // real position for smoother tracking overall
	yPos += vy;
	destRect.x = static_cast<int>(xPos); // sprite postition
	destRect.y = static_cast<int>(yPos);
	switch (type) {
	case Bullettype::ENEMY_RICE:
	case Bullettype::ENEMY_KUNAI_RD:
	case Bullettype::ENEMY_KUNAI_BL: // hitboxes for small
		hitbox.x = destRect.x + 12;
		hitbox.y = destRect.y + 12;
		break;
	case Bullettype::ENEMY_KNIFE:
		hitbox.x = destRect.x + 24; // big
		hitbox.y = destRect.y + 24;
		break;
	case Bullettype::ENEMY_ROUND1:
		hitbox.x = destRect.x + 10;
		hitbox.y = destRect.y + 10;
		break;
	default:
		hitbox.x = destRect.x; // def case
		hitbox.y = destRect.y;
		break;
	}
}

void Bullet::render() {
	static double plyb_angle = 0.0;
	plyb_angle += 0.01;
	if (plyb_angle >= 360.0) plyb_angle = 0.0;
	switch (type) {
		case Bullettype::PLAYER_0:
			SDL_SetTextureAlphaMod(playerbullet_text, 100);
			SDL_RenderCopyEx(Game::Grenderer, playerbullet_text, &srcRect, &destRect, plyb_angle, nullptr, SDL_FLIP_NONE); // bullets rotate
			break;
		case Bullettype::PLAYER_1:
			SDL_SetTextureAlphaMod(playerbullet_text, 100);
			SDL_RenderCopy(Game::Grenderer, playerbullet_text, &srcRect, &destRect); // needles
			break;
		case Bullettype::ENEMY_RICE:
		case Bullettype::ENEMY_KUNAI_RD:
		case Bullettype::ENEMY_KNIFE:
		case Bullettype::ENEMY_KUNAI_BL:
		case Bullettype::ENEMY_ROUND1:
			double angle = atan2(vy, vx) * 180 / M_PI; // get direction 
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

SDL_Rect Bullet::getHitbox() const {
	return hitbox;
}

void Bullet::GrazeUpdate() {
	grazed = true;
}

bool Bullet::getGrazeState() const {
	return grazed;
}

int Bullet::getDmg() const {
	return dmg;
}
