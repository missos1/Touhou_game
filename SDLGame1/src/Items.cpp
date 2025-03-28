#include "headers/Items.hpp"
#include "headers/Player.hpp"
#include "headers/Game.hpp"
#include "headers/Enemy.hpp"
#include <iostream>

Item::Item(double x, double y, Itemtype type) :
	hitbox{ 0, 0, 0, 0 }, xPos(x), yPos(y), type(type),
	destRect{ 0, 0, 0, 0 }, srcRect{ 0, 0, 0, 0 }, fallspeed(0), item_point(0) {
	item_text = Game::enemybullet_text; // Get item texture

	srand(time
	(0));
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
		item_point = 10000; // Set item point value
		break;
	case Itemtype::FULLPOWER: // Full power item
		srcRect = { ITEM_FULLPOWER, ITEM_Y, spriteW, spriteH }; // Set source rectangle for texture
		item_point = 1000; // Set item point value
		break;
	case Itemtype::ONEUP: // 1-Up item
		srcRect = { ITEM_ONEUP, ITEM_Y, spriteW, spriteH }; // Set source rectangle for texture
		item_point = 3000; // Set item point value
		break;
	}
	destRect = { static_cast<int>(x), static_cast<int>(y), spriteW * 2, spriteH * 2 }; // Set destination rectangle for rendering
	hitbox = destRect; // Set hitbox
	fallspeed = -5.0; // Set initial fall speed
}

Item::~Item() {
	// Destructor
}

void Item::update() {
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

	destRect.x = static_cast<int>(xPos); // Update destination rectangle x position
	destRect.y = static_cast<int>(yPos); // Update destination rectangle y position
	hitbox.x = destRect.x; // Update hitbox x position
	hitbox.y = destRect.y; // Update hitbox y position
}

void Item::render() {
	SDL_RenderCopy(Game::Grenderer, item_text, &srcRect, &destRect); // Render item
}

