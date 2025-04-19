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
	animation();
	updatePhase(bullets, items);

	xPos += vx;
	yPos += vy;

	destRect.x = static_cast<int>(xPos);
	destRect.y = static_cast<int>(yPos);

	hitbox.x = destRect.x + 16;
	hitbox.y = destRect.y + 16;
}

void Boss::updatePhase(std::vector<Bullet*>& bullets, std::vector<Item*>& items) {
	if (currentHP <= 0 && phase != Phase::IDLE) {
		state = BossState::RETURNING;
		switch (phase) {
		case Phase::PHASE0:
			setPhase(Phase::PHASE0_SC);
			items.emplace_back(new Item(destRect.x + spriteW_center, destRect.y + spriteH_center, Itemtype::ONEUP));
			SoundManager::PlaySound("enemypowerup1", 0, Game::SE_volume);
			break;

		case Phase::PHASE0_SC:
			setPhase(Phase::PHASE1);
			items.emplace_back(new Item(destRect.x + spriteW_center, destRect.y + spriteH_center, Itemtype::ONEUP));
			items.emplace_back(new Item(destRect.x + spriteW_center + 10, destRect.y + spriteH_center - 10, Itemtype::FULLPOWER));
			SoundManager::PlaySound("enemypowerup0", 0, Game::SE_volume);
			break;

		case Phase::PHASE1:
			setPhase(Phase::PHASE1_SC);
			SoundManager::PlaySound("enemypowerup1", 0, Game::SE_volume);
			items.emplace_back(new Item(destRect.x + spriteW_center, destRect.y + spriteH_center, Itemtype::ONEUP));
			break;
		case Phase::PHASE1_SC:
			//std::cout << "Boss defeated!" << std::endl;
			//SoundManager::PlaySound("endie0", 5, Game::SE_volume);
			SoundManager::PlaySound("endie1", 0, Game::SE_volume);
			resetValue();
			break;
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

	//SDL_SetRenderDrawColor(Game::Grenderer, 0, 255, 0, 255); // debug
	//SDL_RenderFillRect(Game::Grenderer, &hitbox);
}


// Boss' miscellaneous

void Boss::animation() {
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
		return;
	}
	
	if (frameTime >= 1.0f && isIdle) { // idle animation
		frameTime = 0.0f;
		currentFrameIdle = (currentFrameIdle + 1) % totalFrames;
		srcRect.y = 0;
		srcRect.x = currentFrameIdle * spriteW;
	}
	
}

void Boss::resetValue() {
	xPos = BOSS_OG_X;
	yPos = BOSS_OG_Y;
	isIdle = true;
	phase = Phase::IDLE;
	state = BossState::NORMAL;
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
		//std::cout << "Phase " << static_cast<int>(phase) << " defeated!" << std::endl;
	}
}

void Boss::checkforbonus(Player* player) {
	if (player->getPlayerhp() < playerhp_track) Bonus = false;
}

void Boss::clearScreen(std::vector<Bullet*>& bullets, std::vector<Item*>& items) {
	for (int i = (int)bullets.size() - 1; i >= 0; i--) {
		double bullet_xPos = bullets[i]->getX();
		double bullet_yPos = bullets[i]->getY();
		if( bullet_yPos <= PLAY_AREA_Y_MAX
			&& bullet_yPos >= PLAY_AREA_Y_MIN
			&& bullet_xPos <= PLAY_AREA_X_MAX
			&& bullet_xPos >= PLAY_AREA_X_MIN)
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


// Boss' movement behaviors below

void Boss::moveinscreen() {
	
	if (xPos >= 400 && yPos == 120) {
		phase = Phase::PHASE0;
		currentHP = phaseHP[Phase::PHASE0];
		vx = 0;
		vy = 0;
		return;
	}

	movement_returning(410, 120, 10.0);
	
}

void Boss::movement_returning(double targetX, double targetY, double speed) {
	const double threshold = 10.0; 

	double dx = targetX - xPos;
	double dy = targetY - yPos;
	double distance = sqrt(dx * dx + dy * dy);

	if (distance <= threshold) {
		vx = 0;
		vy = 0;
		state = BossState::NORMAL;
		return;
	}

	double angle = atan2(dy, dx);
	vx = cos(angle) * speed;
	vy = sin(angle) * speed;
	
}

void Boss::movement_random(double speed, Uint64 idle_cooldown) {
	static Uint64 stationaryTime = 0;

	static bool stationary = true;
	static bool moving = false;
	static bool justspawned = true;

	static int rnd_new_xPos;
	static int rnd_new_yPos;

	if (justspawned) {
		justspawned = false;
		stationaryTime = Game::GamecurrentTime;
	}

	if (stationary) {
		vx = 0;
		vy = 0;
		if (Game::GamecurrentTime - stationaryTime >= idle_cooldown) {
			stationary = false;
			rnd_new_xPos = rand() % 600 + 50;
			rnd_new_yPos = rand() % 200 + 40;
		}
		return;
	}
	
	const double threshold = 15.0;

	double dx = rnd_new_xPos - xPos;
	double dy = rnd_new_yPos - yPos;
	double distance = sqrt(dx * dx + dy * dy);

	if (distance <= threshold) {
		vx = 0;
		vy = 0;
		stationary = true;
		stationaryTime = Game::GamecurrentTime;
		return;
	}

	double angle = atan2(dy, dx);
	vx = cos(angle) * speed;
	vy = sin(angle) * speed;
}


// Boss' phases below

void Boss::phase0(std::vector<Bullet*>& bullets, Player* player) {
	movement_random(4.0, 1000);
	pattern0(bullets, player);
}

void Boss::phase0_spellcard(std::vector<Bullet*>& bullets, Player* player) {
	if (state == BossState::RETURNING) {
		movement_returning(410, 180, 4.0);
		return;
	}
	
	pattern0_spellcard(bullets, player);
	movement_random(4.0, 10000);
	
}

void Boss::phase1(std::vector<Bullet*>& bullets, Player* player) {
	if (state == BossState::RETURNING) {
		movement_returning(410, 180, 4.0);
		return;
	}
	
}

void Boss::phase1_spellcard(std::vector<Bullet*>& bullets, Player* player) {
	if (state == BossState::RETURNING) {
		movement_returning(410, 180, 4.0);
		return;
	}
	
}


// Boss shooting pattern below

void Boss::pattern0(std::vector<Bullet*>& bullets, Player* player) {
	//static bool reversed = false;
	static Uint64 lastshootTime_0 = 0;
	static Uint64 lastshootTime_1 = 0;

	if (Game::GamecurrentTime - lastshootTime_0 >= 700) {

		circleroundShoot(bullets, 60, Bullettype::ENEMY_RICE_RD, 3.5);
		lastshootTime_0 = Game::GamecurrentTime;
		SoundManager::PlaySound("enshoot1", 0, Game::SE_volume / 2);
	}

	if (Game::GamecurrentTime - lastshootTime_1 >= 3000) {

		std::vector<double> spd = { 4.0, 4.5, 5.0, 5.5 };

		for (const double& i : spd) {
			aimedcircleroundShoot(bullets, 5, Bullettype::ENEMY_RICE_GR, player->getX(), player->getY(), i);
		}

		lastshootTime_1 = Game::GamecurrentTime;
		//SoundManager::PlaySound("enshoot1", 0, Game::SE_volume / 2);
	}

}

void Boss::pattern0_spellcard(std::vector<Bullet*>& bullets, Player* player) {
	if (!isIdle) return;
	static Uint64 lastshootTime_total = 0;
	static Uint64 downTime = 0;
	static Uint64 lastshootTime_0 = 0;
	static bool reversed = false;
	static bool shooting = false;

	if (!shooting && Game::GamecurrentTime - downTime >= 4000) {
		shooting = true;
		lastshootTime_total = Game::GamecurrentTime;
		downTime = Game::GamecurrentTime;
	}

	if (shooting && Game::GamecurrentTime - lastshootTime_total >= 2000) {
		shooting = false;
	}

	if (Game::GamecurrentTime - lastshootTime_0 >= 50 && shooting) {
		spiralShoot(bullets, 8, Bullettype::ENEMY_KNIFE, 5.0, reversed);
		lastshootTime_0 = Game::GamecurrentTime;
		SoundManager::PlaySound("enshoot1", 0, Game::SE_volume / 2);
	}
	
}


// Boss shooting functions below

void Boss::aimedShoot(std::vector<Bullet*>& bullets, Bullettype type, int playerX, int playerY, double speed) {
	double deltax = playerX - destRect.x;
	double deltay = playerY - destRect.y;

	double angle = atan2(static_cast<long double>(deltay), static_cast<long double>(deltax));

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

	double base_angle = atan2(static_cast<long double>(deltay), static_cast<long double>(deltax));
	
	for (int i = 0; i < density; ++i) {
		double angle = base_angle + ((2 * M_PI / density) * i);
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


