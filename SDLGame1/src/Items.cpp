#include "headers/Items.hpp"
#include "headers/Player.hpp"
#include "headers/Game.hpp"
#include "headers/Enemy.hpp"
#include <iostream>

Item::Item(double x, double y, Itemtype type) :
	hitbox{ 0, 0, 0, 0 }, xPos(x), yPos(y), type(type),
	destRect{ 0, 0, 0, 0 }, srcRect{ 0, 0, 0, 0 }, fallspeed(0), item_point(0), TrackPlayer(false) {
	item_text = Game::enemybullet_text; // Get item texture

	switch (type) {
	case Itemtype::POWER_S: // Small power item
		srcRect = { ITEM_POWER_S, ITEM_Y, spriteW, spriteH }; // Set source rectangle for texture
		item_point = 10; // Set item point value
		break;
	case Itemtype::POWER_L: // Large power item
		srcRect = { ITEM_POWER_L, ITEM_Y, spriteW, spriteH }; // Set source rectangle for texture
		item_point = 100; // Set item point value
		break;
	case Itemtype::POINT: // Point item
		srcRect = { ITEM_POINT, ITEM_Y, spriteW, spriteH }; // Set source rectangle for texture
		item_point = 20000; // Set item point value
		break;
	case Itemtype::FULLPOWER: // Full power item
		srcRect = { ITEM_FULLPOWER, ITEM_Y, spriteW, spriteH }; // Set source rectangle for texture
		item_point = 1000; // Set item point value
		break;
	case Itemtype::ONEUP: // 1-Up item
		srcRect = { ITEM_ONEUP, ITEM_Y, spriteW, spriteH }; // Set source rectangle for texture
		item_point = 3000; // Set item point value
		break;
	case Itemtype::STAR:
		srcRect = { ITEM_STAR, ITEM_Y, spriteW, spriteH }; // Set source rectangle for texture
		item_point = 1000; // Set item point value
		break;
	default:
		break;
	}
	destRect = { static_cast<int>(x), static_cast<int>(y), spriteW * 2, spriteH * 2 }; // Set destination rectangle for rendering
	hitbox = destRect; // Set hitbox
	fallspeed = -5.0; // Set initial fall speed
}

Item::~Item() {
	// Destructor
}

void Item::update(Player* player) {
	if ((player->getY() <= 250 || type == Itemtype::STAR) 
		&& yPos <= PLAY_AREA_Y_MAX
		&& yPos >= PLAY_AREA_Y_MIN
		&& xPos <= PLAY_AREA_X_MAX
		&& xPos >= PLAY_AREA_X_MIN) TrackPlayer = true;

	if (TrackPlayer) {
		double deltax = player->getX() - destRect.x;
		double deltay = player->getY() - destRect.y;
		double angle = atan2(static_cast<long double>(deltay), static_cast<long double>(deltax));
		xPos += cos(angle) * 20;
		yPos += sin(angle) * 20;
	}
	else {
		fallspeed += 0.05; // Increase fall speed
		if (fallspeed >= 5) fallspeed = 5.0; // Cap fall speed

		if (fallspeed <= 0) {
			double angle = (rand() % 60 + 60) * M_PI / 180.0;;
			xPos += cos(angle) * fallspeed;
			yPos += sin(angle) * fallspeed;
		}
		else {
			xPos += 0.0; // Update x position (no change)
			yPos += fallspeed; // Update y position
		}
	}
		

	destRect.x = static_cast<int>(xPos); // Update destination rectangle x position
	destRect.y = static_cast<int>(yPos); // Update destination rectangle y position
	hitbox.x = destRect.x; // Update hitbox x position
	hitbox.y = destRect.y; // Update hitbox y position
}

void Item::render() {
	SDL_RenderCopy(Game::Grenderer, item_text, &srcRect, &destRect); // Render item
}

