#include "headers/Boss.hpp"
#include "headers/Bullets.hpp"
#include "headers/Player.hpp"
#include "headers/Items.hpp"
#include "headers/Game.hpp"


Boss::Boss(double x, double y)
	: xPos(x), yPos(y), destRect{ 0, 0, 0, 0 }, srcRect{ 0, 0, 0, 0 }, isIdle(true),
	vx(0), vy(0), hitbox{ 0, 0, 0, 0 }, phase(Phase::IDLE), state(BossState::NORMAL) {

	Boss_texture = TextureManager::LoadTexture("res/Enemy/Flandre.png");
	Circle_texture = TextureManager::LoadTexture("res/Enemy/circle.png");
	Spellcard_texture = TextureManager::LoadTexture("res/Enemy/Flandre_SpellCard.png");

	totalFrames = 4;
	Ani_speed = 0.1f;
	frameTime = 0.0f;
	currentFrame = 0;
	currentFrameIdle = 0;

	phaseHP[Phase::IDLE] = 1;
	phaseHP[Phase::PHASE0] = 500;
	phaseHP[Phase::PHASE0_SC] = 3000;
	phaseHP[Phase::PHASE1] = 3000;
	phaseHP[Phase::PHASE1_SC] = 4000;

	srcRect = { 0, 0, spriteW, spriteH };
	srcRect_circle = { 0, 0, 60, 59 };
	destRect = { static_cast<int>(x), static_cast<int>(y), spriteW * 2, spriteH * 2 };
	hitbox = { destRect.x + 16, destRect.y + 16, 96, 96 };


	int w, h;
	SDL_QueryTexture(Spellcard_texture, nullptr, nullptr, &w, &h);
	destRect_spellcard = { 0, 30, static_cast<int>(w * 3.5), static_cast<int>(h * 3.5) };
}

Boss::~Boss() {
	if (Boss_texture) {
		SDL_DestroyTexture(Boss_texture);
		Boss_texture = nullptr;
	}
	if (Circle_texture) {
		SDL_DestroyTexture(Circle_texture);
		Circle_texture = nullptr;
	}
}

void Boss::update(std::vector<Bullet*>& bullets, std::vector<Item*>& items) {
	if (vx != 0 || vy != 0) {
		if (vx > 0) isFlipped = true;
		else isFlipped = false;

		isIdle = false;
		isMoving = true;
	}
	else if (vx == 0 && vy == 0) {
		isIdle = true;
	}

	frameTime += Ani_speed;

	if (isMoving) {
		if (currentFrame < 1 && !isIdle) { // left right animation  
			if (frameTime >= 1.5f) { // if less than 1 then increment to next fram
				frameTime = 0.0f;
				currentFrame++;
				if (currentFrame >= 1) currentFrame = 1;
			}
		}
		else if (currentFrame > 0 && isIdle) { // if is idling after moving then buffers to slowly play frame back
			if (frameTime >= 0.1f) {
				frameTime = 0.0f;
				currentFrame--;
			}
		}
		else if (currentFrame == 0 && isIdle) { // after playing animation, return to idle animation
			isMoving = false;
		}
		srcRect.y = spriteH;
		srcRect.x = currentFrame * spriteW;
	}
	else {
		if (frameTime >= 1.0f && isIdle) { // idle animation
			frameTime = 0.0f;
			currentFrameIdle = (currentFrameIdle + 1) % totalFrames;
			srcRect.y = 0;
			srcRect.x = currentFrameIdle * spriteW;
		}
	}

	xPos += vx;
	yPos += vy;

	destRect.x = static_cast<int>(xPos);
	destRect.y = static_cast<int>(yPos);

	hitbox.x = destRect.x + 16;
	hitbox.y = destRect.y + 16;

	updatePhase(bullets, items);
}

void Boss::updatePhase(std::vector<Bullet*>& bullets, std::vector<Item*>& items) {
	if (currentHP <= 0 && phase != Phase::IDLE) {
		state = BossState::RETURNING;
		if (phase == Phase::PHASE0) {
			setPhase(Phase::PHASE0_SC);
			SoundManager::PlaySound("enemypowerup1", 0, Game::SE_volume);
		}
		else if (phase == Phase::PHASE0_SC) {
			setPhase(Phase::PHASE1);
			SoundManager::PlaySound("enemypowerup0", 0, Game::SE_volume);
		}
		else if (phase == Phase::PHASE1) {
			setPhase(Phase::PHASE1_SC);
			SoundManager::PlaySound("enemypowerup1", 0, Game::SE_volume);
		}
		else if (phase == Phase::PHASE1_SC) {
			std::cout << "Boss defeated!" << std::endl;
			SoundManager::PlaySound("endie0", 5, Game::SE_volume);
			SoundManager::PlaySound("endie1", 0, Game::SE_volume);
			//phase = Phase::IDLE; // Reset to idle or handle boss defeat
			xPos = BOSS_OG_X;
			yPos = BOSS_OG_Y;
		}
		clearScreen(bullets, items);
	}
}

void Boss::render() {
	static int spellcard_opacity = 0;
	if (phase == Phase::PHASE0_SC || phase == Phase::PHASE1_SC) {
		SDL_SetTextureAlphaMod(Spellcard_texture, spellcard_opacity);
		SDL_RenderCopy(Game::Grenderer, Spellcard_texture, nullptr, &destRect_spellcard);
		spellcard_opacity += 5;
		if (spellcard_opacity >= 255) spellcard_opacity = 255;
	}
	else {
		spellcard_opacity = 0;
	}
	
	SDL_RenderCopyEx(Game::Grenderer, Boss_texture, &srcRect, &destRect, 0, nullptr, isFlipped ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL);

	static int angle = 0;
	if (Game::state != GameState::PAUSE) angle = (angle + 1 + 360) % 360;
	if (Boss::phase != Phase::IDLE) TextureManager::render_from_texture(Circle_texture, destRect.x - 55, destRect.y - 40, 4, angle, SDL_FLIP_NONE);

	SDL_SetRenderDrawColor(Game::Grenderer, 0, 255, 0, 255); // debug
	SDL_RenderFillRect(Game::Grenderer, &hitbox);
}

void Boss::resetValue() {
	xPos = BOSS_OG_X;
	yPos = BOSS_OG_Y;
	phase = Phase::IDLE;
	vx = vy = 0;
}

void Boss::setPhase(Phase newPhase) {
	phase = newPhase;
	currentHP = phaseHP[phase]; // Update HP for the new phase
}

void Boss::takeDamage(int damage) {
	if (phase != Phase::IDLE) currentHP -= damage;
	if (currentHP <= 0) {
		currentHP = 0;
		// Handle phase transition or boss defeat here
		std::cout << "Phase " << static_cast<int>(phase) << " defeated!" << std::endl;
	}
}

void Boss::checkforbonus(Player* player) {
	if (player->getPlayerhp() < playerhp_track) Bonus = false;
}

void Boss::clearScreen(std::vector<Bullet*>& bullets, std::vector<Item*>& items) {
	for (int i = (int)bullets.size() - 1; i >= 0; i--) {
		double bullet_xPos = bullets[i]->getX();
		double bullet_yPos = bullets[i]->getY();

		items.emplace_back(new Item(bullet_xPos - 5, bullet_yPos - 5, Itemtype::STAR)); // Generate star items at cleared bullets pos
		
		delete bullets[i]; // Delete bullet
		bullets.erase(bullets.begin() + i); // Remove bullet from vector
	}
}

void Boss::debug_ani(const Uint8* keys) {
	vx = vy = 0;
	if (keys[SDL_SCANCODE_UP]) vy = -4;
	if (keys[SDL_SCANCODE_DOWN]) vy = 4;
	if (keys[SDL_SCANCODE_LEFT] || keys[SDL_SCANCODE_RIGHT]) {
		if (keys[SDL_SCANCODE_LEFT]) vx = -4;
		if (keys[SDL_SCANCODE_RIGHT])  vx = 4;
	}
}


// Boss' phases behaviors below

void Boss::moveinscreen() {
	if (xPos >= 410 && yPos == 120) {
		phase = Phase::PHASE0;
		currentHP = phaseHP[Phase::PHASE0];
		vx = 0;
		vy = 0;
		xPos = 410;
		yPos = 120;
	}
	else {
		vx = 10;
	}
}

void Boss::move_returning(double targetX, double targetY) {
	const double threshold = 20.0; 

	double dx = targetX - xPos;
	double dy = targetY - yPos;
	double distance = sqrt(dx * dx + dy * dy);

	if (distance <= threshold) {
		vx = 0;
		vy = 0;
		xPos = targetX;
		yPos = targetY;
		state = BossState::NORMAL;
	}
	else {
		double angle = atan2(dy, dx);
		vx = cos(angle) * 4.0;
		vy = sin(angle) * 4.0;
	}
}

void Boss::phase0(std::vector<Bullet*>& bullets, Player* player) {
	const double leftLimit = 200;
	const double rightLimit = 600;
	const double speed = 4.0;

	static Uint64 stationaryTime = 0;


	static bool stationary = true;
	static bool movingRight = false;
	static bool justspawned = true;

	if (justspawned) {
		justspawned = false;
		stationaryTime = Game::GamecurrentTime;
	}

	// Move left and right between leftLimit and rightLimit
	if (stationary) {
		vx = 0;
		vy = 0;
		if (Game::GamecurrentTime - stationaryTime >= 3000) {
			stationary = false;
		}
	}
	else if (movingRight) {
		vx = speed;
		if (xPos >= rightLimit) {
			movingRight = false;
			stationary = true;
			stationaryTime = Game::GamecurrentTime;
		}
	}
	else {
		vx = -speed;
		if (xPos <= leftLimit) {
			movingRight = true;
			stationary = true;
			stationaryTime = Game::GamecurrentTime;
		}
	}

	vy = 0; // stay at the same Y position

	pattern0(bullets, player);
}

void Boss::phase0_spellcard(std::vector<Bullet*>& bullets, Player* player) {
	if (state == BossState::RETURNING) {
		move_returning(410, 180);
	}
	else {
		pattern0_spellcard(bullets, player);
	}
}


// Boss shooting pattern below

void Boss::pattern0(std::vector<Bullet*>& bullets, Player* player) {
	//static bool reversed = false;
	static Uint64 lastshootTime_0 = 0;

	if (Game::GamecurrentTime - lastshootTime_0 >= 400) {
		
		circleroundShoot(bullets, 20, Bullettype::ENEMY_RICE_BL, 3.5);
		circleroundShoot(bullets, 38, Bullettype::ENEMY_RICE_GR, 3.3);
		lastshootTime_0 = Game::GamecurrentTime;
		SoundManager::PlaySound("enshoot1", 0, Game::SE_volume / 2);
	}	
}

void Boss::pattern0_spellcard(std::vector<Bullet*>& bullets, Player* player) {
	static Uint64 lastshootTime_0 = 0;
	static Uint64 lastshootTime_1 = 0;
	static bool reversed = false;
	static bool shooting = false;

	if (Game::GamecurrentTime - lastshootTime_1 >= 500) {
		shooting = !shooting;
		reversed = !reversed;
		lastshootTime_1 = Game::GamecurrentTime;
	}

	if (Game::GamecurrentTime - lastshootTime_0 >= 50 && shooting) {
		spiralShoot(bullets, 10, Bullettype::ENEMY_KNIFE, 5.0, reversed);
		lastshootTime_0 = Game::GamecurrentTime;
		SoundManager::PlaySound("enshoot1", 0, Game::SE_volume / 2);
	}
}

// Boss shooting functions below

void Boss::aimedShoot(std::vector<Bullet*>& bullets, Bullettype type, int playerX, int playerY, double speed) {
	double deltax = playerX - destRect.x;
	double deltay = playerY - destRect.y;

	double angle = atan2((long double)deltay, (long double)deltax);

	std::vector<double> buffer = { -M_PI / 6, 0, M_PI / 6 };
	//std::vector<double> spdvar = { 3.6, 4, 4.2, 4.5, 5, 6, 7 ,8 };
	for (const double& offset : buffer) {
		double velx = cos(angle) * speed;
		double vely = sin(angle) * speed;
		bullets.emplace_back(new Bullet(destRect.x + spriteW_center, destRect.y + spriteH_center, velx, vely, type));
	}
}

void Boss::circleroundShoot(std::vector<Bullet*>& bullets, int density, Bullettype type, double speed) {
	//int m = 10;
	double offset = rand() % 50;
	offset /= 10;
	for (int i = 0; i < density; ++i) {
		double angle = (2 * M_PI / density) * i;
		//std::vector<double> spdvar = { 5 };
		//for (double speed : spdvar) {
			double velx = cos(angle+offset) * speed;
			double vely = sin(angle+offset) * speed;
			bullets.emplace_back(new Bullet(destRect.x + spriteW_center, destRect.y + spriteH_center, velx, vely, type));
		//}
	}
}

void Boss::aimedcircleroundShoot(std::vector<Bullet*>& bullets, int density, Bullettype type, int playerX, int playerY, double speed) {
	double deltax = playerX - destRect.x;
	double deltay = playerY - destRect.y;

	double angle = atan2((long double)deltay, (long double)deltax);
	
	for (int i = 0; i < density; ++i) {
		angle += (2 * M_PI / density) * i;
		double velx = cos(angle) * speed;
		double vely = sin(angle) * speed;
		bullets.emplace_back(new Bullet(destRect.x + spriteW_center, destRect.y + spriteH_center, velx, vely, type));
	}
}

void Boss::spiralShoot(std::vector<Bullet*>& bullets, int density, Bullettype type, double speed, bool Reversed) {
	static double offset = 0;
	if (offset >= 360) offset = 0;
	
	for (int i = 0; i < density; ++i) {
		double angle = (2 * M_PI / density) * i;
		//std::vector<double> spdvar = { 5 };
		//for (double speed : spdvar) {
		double velx = cos(angle + (Reversed ? -offset : offset)) * speed;
		double vely = sin(angle + (Reversed ? -offset : offset)) * speed;
		bullets.emplace_back(new Bullet(destRect.x + spriteW_center, destRect.y + spriteH_center, velx, vely, type));
		//}
	}
	offset++;
}

void Boss::rndShoot(std::vector<Bullet*>& bullets, int density, Bullettype type, double speed) {
	//int density = 10;

	for (int i = 0; i < density; i++) {
		double angle = (std::rand() % 180) * M_PI / 180;
		double tmpspeed = speed + std::rand() % 2;
		double velx = cos(angle) * tmpspeed;
		double vely = sin(angle) * tmpspeed;
		bullets.emplace_back(new Bullet(destRect.x + spriteW_center, destRect.y + spriteH_center, velx, vely, type));
	}
}



