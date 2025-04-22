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
		&& yPos >= PLAY_AREA_Y_MIN - 100
		&& xPos <= PLAY_AREA_X_MAX + 20
		&& xPos >= PLAY_AREA_X_MIN - 20) TrackPlayer = true;

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

void Item::enemy_drop(Enemy* enemy, std::vector<Item*>& items, Player* player) {
	SDL_Rect enemy_hitbox = enemy->getEnHitbox(); // Get enemy's hitbox
	Itemtype type; // Item type

	SoundManager::PlaySound("endie0", 0, Game::SE_volume / 4); // Play enemy death sound

	if (enemy->getType() == EnemyType::RED_FA ||
		enemy->getType() == EnemyType::WHITE_FA ||
		enemy->getType() == EnemyType::BLUE_FA) {
		int loop = 0; // Number of items to spawn based on enemy type

		switch (enemy->getType()) {
		case EnemyType::RED_FA:
			loop = 3; // Number of items to spawn
			break;
		case EnemyType::WHITE_FA:
			loop = 10; // Number of items to spawn
			break;
		case EnemyType::BLUE_FA:
			loop = 5; // Number of items to spawn
			break;
		}

		for (int k = 0; k < loop; ++k) {
			int randomX = randomspawn(301, -150, enemy_hitbox.x);  // Random X offset between -150 and 150
			int randomY = randomspawn(301, -150, enemy_hitbox.y);  // Random Y offset between -150 and 150
			items.emplace_back(new Item(randomX, randomY, Itemtype::POINT));
		}

		items.emplace_back(new Item(enemy_hitbox.x, enemy_hitbox.y, Itemtype::POWER_L)); // Spawn a large power item

		if (enemy->getType() == EnemyType::WHITE_FA) {
			int randomX = randomspawn(301, -150, enemy_hitbox.x);  // Random X offset between -150 and 150
			int randomY = randomspawn(301, -150, enemy_hitbox.y);  // Random Y offset between -150 and 150
			items.emplace_back(new Item(randomX, randomY, Itemtype::POWER_L)); // Spawn a large power item
		}

		return;
	}

	static int countspawn = 1; // Count for item spawn

	type = Itemtype::POINT; // Default item type

	if (player->getPlayerpowerlv() < 5.0) { // Determine item type
		if (countspawn % 31 == 0) {
			type = Itemtype::POWER_L;
			countspawn = 1;
		}
		else if (countspawn % 3 == 0) {
			type = Itemtype::POWER_S;
		}
		countspawn++;
	}

	items.emplace_back(new Item(enemy_hitbox.x, enemy_hitbox.y, type)); // Spawn item
}

void Item::boss_drop(Boss* boss, std::vector<Item*>& items) {

}