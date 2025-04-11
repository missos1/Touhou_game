#include "headers/Bullets.hpp"
#include "headers/Game.hpp"
#include <iostream>
#include <cmath>

Bullet::Bullet(double x, double y, double velx, double vely, Bullettype type)
	: xPos(x), yPos(y), vx(velx), vy(vely), type(type),
	destRect{ 0, 0, 0, 0 }, srcRect{ 0, 0, 0, 0 }, // Initialize variables
	playerbullet_text(nullptr), enemybullet_text(nullptr),
	savedVx(0), savedVy(0), hitbox{ 0, 0, 0, 0 }, dmg(0) {

	int spriteW = 0, spriteH = 0, spriteXpos = 0, spriteYpos = 0;
	playerbullet_text = Game::Misc_player_text; // Get player bullet texture
	enemybullet_text = Game::enemybullet_text; // Get enemy bullet texture

	switch (type) {
	case Bullettype::PLAYER_0: // Red talisman
		spriteXpos = 132; spriteYpos = 0; spriteW = 16; spriteH = 16;
		srcRect = { spriteXpos, spriteYpos, spriteW, spriteH };
		destRect = { static_cast<int>(x), static_cast<int>(y), spriteW * 3, spriteH * 3 };
		hitbox = destRect;
		dmg = 1;
		break;
	case Bullettype::PLAYER_1: // Exterminate needles
		spriteXpos = 200; spriteYpos = 0; spriteW = 9; spriteH = 48;
		srcRect = { spriteXpos, spriteYpos, spriteW, spriteH };
		destRect = { static_cast<int>(x), static_cast<int>(y), spriteW * 2, spriteH * 2 };
		hitbox = destRect;
		dmg = 2;
		break;
	case Bullettype::ENEMY_KUNAI_RD: // Red kunai
		spriteXpos = DEFAULT_X + L_RED; spriteYpos = KUNAI_Y; spriteW = 16; spriteH = 16;
		srcRect = { spriteXpos, spriteYpos, spriteW, spriteH };
		destRect = { static_cast<int>(x), static_cast<int>(y), spriteW * 2, spriteH * 2 };
		hitbox = { static_cast<int>(y), static_cast<int>(x), S_HITBOX_SIZE , S_HITBOX_SIZE };
		break;
	case Bullettype::ENEMY_KUNAI_BL: // Blue kunai
		spriteXpos = DEFAULT_X + L_BLUE; spriteYpos = KUNAI_Y; spriteW = 16; spriteH = 16;
		srcRect = { spriteXpos, spriteYpos, spriteW, spriteH };
		destRect = { static_cast<int>(x), static_cast<int>(y), spriteW * 2, spriteH * 2 };
		hitbox = { static_cast<int>(y), static_cast<int>(x), S_HITBOX_SIZE , S_HITBOX_SIZE };
		break;
	case Bullettype::ENEMY_KUNAI_GR: // Blue kunai
		spriteXpos = DEFAULT_X + L_GREEN; spriteYpos = KUNAI_Y; spriteW = 16; spriteH = 16;
		srcRect = { spriteXpos, spriteYpos, spriteW, spriteH };
		destRect = { static_cast<int>(x), static_cast<int>(y), spriteW * 2, spriteH * 2 };
		hitbox = { static_cast<int>(y), static_cast<int>(x), S_HITBOX_SIZE , S_HITBOX_SIZE };
		break;
	case Bullettype::ENEMY_KNIFE: // Knife
		spriteXpos = DEFAULT_X + L_BLUE; spriteYpos = KNIFE_Y; spriteW = 32; spriteH = 32;
		srcRect = { spriteXpos, spriteYpos, spriteW, spriteH };
		destRect = { static_cast<int>(x), static_cast<int>(y), spriteW * 2, spriteH * 2 };
		hitbox = { static_cast<int>(y), static_cast<int>(x), L_HITBOX_SIZE , L_HITBOX_SIZE };
		break;
	case Bullettype::ENEMY_RICE_RD: // Rice bullet
		spriteXpos = DEFAULT_X + L_RED; spriteYpos = RICE_Y; spriteW = 16; spriteH = 16;
		srcRect = { spriteXpos, spriteYpos, spriteW, spriteH };
		destRect = { static_cast<int>(x), static_cast<int>(y), spriteW * 2, spriteH * 2 };
		hitbox = { static_cast<int>(y), static_cast<int>(x), S_HITBOX_SIZE , S_HITBOX_SIZE };
		break;
	case Bullettype::ENEMY_RICE_BL: // Rice bullet
		spriteXpos = DEFAULT_X + L_BLUE; spriteYpos = RICE_Y; spriteW = 16; spriteH = 16;
		srcRect = { spriteXpos, spriteYpos, spriteW, spriteH };
		destRect = { static_cast<int>(x), static_cast<int>(y), spriteW * 2, spriteH * 2 };
		hitbox = { static_cast<int>(y), static_cast<int>(x), S_HITBOX_SIZE , S_HITBOX_SIZE };
		break;
	case Bullettype::ENEMY_RICE_GR: // Rice bullet
		spriteXpos = DEFAULT_X + L_GREEN; spriteYpos = RICE_Y; spriteW = 16; spriteH = 16;
		srcRect = { spriteXpos, spriteYpos, spriteW, spriteH };
		destRect = { static_cast<int>(x), static_cast<int>(y), spriteW * 2, spriteH * 2 };
		hitbox = { static_cast<int>(y), static_cast<int>(x), S_HITBOX_SIZE , S_HITBOX_SIZE };
		break;
	case Bullettype::ENEMY_ROUND1: // Round bullet
		spriteXpos = DEFAULT_X + L_BLUE; spriteYpos = ROUND1_Y; spriteW = 16; spriteH = 16;
		srcRect = { spriteXpos, spriteYpos, spriteW, spriteH };
		destRect = { static_cast<int>(x), static_cast<int>(y), spriteW * 2, spriteH * 2 };
		hitbox = { static_cast<int>(y), static_cast<int>(x), M_HITBOX_SIZE , M_HITBOX_SIZE };
		break;
	}
}

Bullet::~Bullet() {
	// Destructor
}

void Bullet::update() {
	xPos += vx; // Update x position
	yPos += vy; // Update y position
	destRect.x = static_cast<int>(xPos); // Update sprite x position
	destRect.y = static_cast<int>(yPos); // Update sprite y position
	switch (type) {
	case Bullettype::ENEMY_RICE_BL:
	case Bullettype::ENEMY_RICE_GR:
	case Bullettype::ENEMY_RICE_RD:
	case Bullettype::ENEMY_KUNAI_RD:
	case Bullettype::ENEMY_KUNAI_BL: // Small hitboxes
	case Bullettype::ENEMY_KUNAI_GR: // Small hitboxes
		hitbox.x = destRect.x + 12;
		hitbox.y = destRect.y + 12;
		break;
	case Bullettype::ENEMY_KNIFE: // Large hitbox
		hitbox.x = destRect.x + 24;
		hitbox.y = destRect.y + 24;
		break;
	case Bullettype::ENEMY_ROUND1: // Medium hitbox
		hitbox.x = destRect.x + 10;
		hitbox.y = destRect.y + 10;
		break;
	default: // Default case
		hitbox.x = destRect.x;
		hitbox.y = destRect.y;
		break;
	}
}

void Bullet::render() {
	static double plyb_angle = 0.0;
	if (Game::state != GameState::PAUSE) {
		plyb_angle += 0.5; // Increment angle for rotation
		if (plyb_angle >= 360.0) plyb_angle = 0.0; // Reset angle if it exceeds 360 degrees
	}
	switch (type) {
	case Bullettype::PLAYER_0:
		SDL_SetTextureAlphaMod(playerbullet_text, 100); // Set transparency
		SDL_RenderCopyEx(Game::Grenderer, playerbullet_text, &srcRect, &destRect, plyb_angle, nullptr, SDL_FLIP_NONE); // Render rotating bullet
		break;
	case Bullettype::PLAYER_1:
		SDL_SetTextureAlphaMod(playerbullet_text, 100); // Set transparency
		SDL_RenderCopy(Game::Grenderer, playerbullet_text, &srcRect, &destRect); // Render needle
		break;
	case Bullettype::ENEMY_RICE_GR:
	case Bullettype::ENEMY_RICE_BL:
	case Bullettype::ENEMY_RICE_RD:
	case Bullettype::ENEMY_KUNAI_RD:
	case Bullettype::ENEMY_KUNAI_GR:
	case Bullettype::ENEMY_KNIFE:
	case Bullettype::ENEMY_KUNAI_BL:
	case Bullettype::ENEMY_ROUND1:
		double angle = atan2(vy, vx) * 180 / M_PI; // Calculate angle for direction
		SDL_RenderCopyEx(Game::Grenderer, enemybullet_text, &srcRect, &destRect, angle - 90, nullptr, SDL_FLIP_NONE); // Render bullet with direction
		break;
	}
	//SDL_SetRenderDrawColor(Game::Grenderer, 0, 255, 0, 255);
	//SDL_RenderFillRect(Game::Grenderer, &hitbox);  // Debug hitboxes
}

