#include "headers/Boss.hpp"
#include "headers/Bullets.hpp"
#include "headers/Player.hpp"
#include "headers/Items.hpp"
#include "headers/Game.hpp"


Boss::Boss(double x, double y)
	: xPos(x), yPos(y), destRect{ 0, 0, 0, 0 }, srcRect{ 0, 0, 0, 0 }, isIdle(true),
	vx(0), vy(0), hitbox{ 0, 0, 0, 0 }, phase(Phase::IDLE), state(BossState::NORMAL),
	currentHP(0), phase_duration(9999999), spellcard_count(3),elapsed(0), phase_time_left(0),
	destRect_circle{ 0, 0, 0, 0 }, text_texture(nullptr), isFlipped(false), isMoving(false) {

	Boss_texture = TextureManager::LoadTexture("res/Enemy/Flandre.png");
	Circle_texture = TextureManager::LoadTexture("res/Enemy/circle.png");
	Spellcard_texture = TextureManager::LoadTexture("res/Enemy/Flandre_SpellCard.png");

	totalFrames = 4;
	Ani_speed = 0.1f;
	frameTime = 0.0f;
	currentFrame = 0;
	currentFrameIdle = 0;

	phase_time_map[Phase::IDLE] = 9999999;
	phase_time_map[Phase::PHASE0] = 30;
	phase_time_map[Phase::PHASE0_SC] = 65;
	phase_time_map[Phase::PHASE1] = 35;
	phase_time_map[Phase::PHASE1_SC] = 75;
	phase_time_map[Phase::PHASE2] = 50;
	phase_time_map[Phase::PHASE2_SC] = 9999999;

	phaseHP[Phase::IDLE] = 0;
	phaseHP[Phase::PHASE0] = 1000;
	phaseHP[Phase::PHASE0_SC] = 2000;
	phaseHP[Phase::PHASE1] = 1200;
	phaseHP[Phase::PHASE1_SC] = 1500;
	phaseHP[Phase::PHASE2] = 1000;
	phaseHP[Phase::PHASE2_SC] = 2400;
	phaseHP[Phase::DEAD] = 0;

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

	phase_time_left = phase_duration - static_cast<int>(elapsed);
}

void Boss::updatePhase(std::vector<Bullet*>& bullets, std::vector<Item*>& items) {
	if ((currentHP <= 0 || phase_time_left <= 0) && phase != Phase::IDLE) {
		if (currentHP <= 0) Item::boss_drop(this, items);
		if (phase_time_left > 0) Game::PLAYSCORE += 100000 * ((phase_time_left > 99) ? 99 : phase_time_left );// score bonus for time left
		state = BossState::RETURNING;
		switch (phase) {
		case Phase::PHASE0:
			setPhase(Phase::PHASE0_SC);
			items.emplace_back(new Item(destRect.x + spriteW_center, destRect.y + spriteH_center, Itemtype::ONEUP));
			SoundManager::PlaySound("enemypowerup1", 0, Game::SE_volume);
			spellcard_count--;  
			break;
		case Phase::PHASE0_SC:
			setPhase(Phase::PHASE1);
			items.emplace_back(new Item(destRect.x + spriteW_center, destRect.y + spriteH_center, Itemtype::ONEUP));
			items.emplace_back(new Item(destRect.x + spriteW_center + 15, destRect.y + spriteH_center - 35, Itemtype::FULLPOWER));
			SoundManager::PlaySound("enemypowerup0", 0, Game::SE_volume);
			break;
		case Phase::PHASE1:
			setPhase(Phase::PHASE1_SC);
			SoundManager::PlaySound("enemypowerup1", 0, Game::SE_volume);
			items.emplace_back(new Item(destRect.x + spriteW_center, destRect.y + spriteH_center, Itemtype::ONEUP));
			spellcard_count--;
			break;
		case Phase::PHASE1_SC:
			setPhase(Phase::PHASE2);
			SoundManager::PlaySound("enemypowerup0", 0, Game::SE_volume);
			items.emplace_back(new Item(destRect.x + spriteW_center, destRect.y + spriteH_center, Itemtype::ONEUP));
			items.emplace_back(new Item(destRect.x + spriteW_center + 15, destRect.y + spriteH_center - 35, Itemtype::FULLPOWER));
			break;
		case Phase::PHASE2:
			setPhase(Phase::PHASE2_SC);
			SoundManager::PlaySound("enemypowerup1", 0, Game::SE_volume);
			items.emplace_back(new Item(destRect.x + spriteW_center, destRect.y + spriteH_center, Itemtype::ONEUP));
			spellcard_count--;
			break;
		case Phase::PHASE2_SC:
			setPhase(Phase::DEAD);
			xPos = BOSS_OG_X;
			yPos = BOSS_OG_Y;
			//std::cout << "Boss defeated!" << std::endl;
			//SoundManager::PlaySound("endie0", 5, Game::SE_volume);
			SoundManager::PlaySound("endie1", 0, Game::SE_volume);
			SoundManager::StopMusic();
			//resetValue();
			break;
		}

		clearScreen(bullets, items);
	}
}

void Boss::render() {
	static int spellcard_opacity = 0;
	if (phase == Phase::PHASE0_SC || phase == Phase::PHASE1_SC || phase == Phase::PHASE2_SC) {
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

	if (!isMoving) {
		if (frameTime >= 1.0f && isIdle) { // idle animation
			frameTime = 0.0f;
			currentFrameIdle = (++currentFrameIdle) % totalFrames;
			srcRect.y = 0;
			srcRect.x = currentFrameIdle * spriteW;
		}
		return;
	}

	if (currentFrame < 1 && !isIdle) { // left right animation  
		if (frameTime >= 1.5f) { // if less than 1 then increment to next fram
			frameTime = 0.0f;
			currentFrame++;
		}
	}
	else if (currentFrame >= 0 && isIdle) { // if is idling after moving then buffers to slowly play frame back
		if (currentFrame == 0 && isIdle) { // after playing animation, return to idle animation
			isMoving = false;
			return;
		}

		if (frameTime >= 0.1f) {
			frameTime = 0.0f;
			currentFrame--;
		}
	}
	
	srcRect.y = spriteH;
	srcRect.x = currentFrame * spriteW;
}

void Boss::resetValue() {
	xPos = BOSS_OG_X;
	yPos = BOSS_OG_Y;
	isIdle = true;
	phase = Phase::IDLE;
	state = BossState::NORMAL;
	spellcard_count = 3;
	vx = vy = 0;

	pattern0_initialized = false;
	pattern0_spellcard_initialized = false;

	pattern1_initialized = false;
	pattern1_spellcard_initialized = false;

	pattern2_initialized = false;
	pattern2_spellcard_initialized = false;
}

void Boss::setPhase(Phase newPhase) {
	phase = newPhase;
	currentHP = phaseHP[phase]; // Update HP for the new phase
	phase_duration = phase_time_map[phase] + elapsed; // Update phase time
}

void Boss::takeDamage(int damage) {
	if (phase != Phase::IDLE) currentHP -= damage;
	if (currentHP <= 0) {
		currentHP = 0;
		// Handle phase transition or boss defeat here
		//std::cout << "Phase " << static_cast<int>(phase) << " defeated!" << std::endl;
	}
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

void Boss::getStage_elapsed(const double& elapsed) {
	this->elapsed = elapsed;
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
	if (xPos >= 380 && yPos == 120) {
		phase = Phase::PHASE0;
		currentHP = phaseHP[Phase::PHASE0];
		phase_duration = phase_time_map[Phase::PHASE0] + elapsed; // Update phase time
		
		vx = 0;
		vy = 0;
		return;
	}

	movement_returning(410, 120, 10.0);
	
}

void Boss::movement_returning(double targetX, double targetY, double speed) {
	const double threshold = 30.0; 

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
			rnd_new_xPos = rand() % 600 + 70;
			rnd_new_yPos = rand() % 200 + 50;
		}
		return;
	}
	
	const double threshold = 30.0;

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
		movement_returning(410, 240, 4.0);
		return;
	}
	pattern0_spellcard(bullets, player);
}

void Boss::phase1(std::vector<Bullet*>& bullets, Player* player) {
	if (state == BossState::RETURNING) {
		movement_returning(410, 180, 4.0);
		return;
	}
	movement_random(4.5, 1000);
	pattern1(bullets, player);
}

void Boss::phase1_spellcard(std::vector<Bullet*>& bullets, Player* player) {
	if (state == BossState::RETURNING) {
		movement_returning(410, 180, 4.0);
		return;
	}
	movement_random(4.5, 4000);
	pattern1_spellcard(bullets, player);
}

void Boss::phase2(std::vector<Bullet*>& bullets, Player* player) {
	if (state == BossState::RETURNING) {
		movement_returning(410, 180, 4.0);
		return;
	}
	movement_random(4.5, 1000);
	pattern2(bullets, player);
}

void Boss::phase2_spellcard(std::vector<Bullet*>& bullets, Player* player) {
	if (state == BossState::RETURNING) {
		movement_returning(410, 180, 4.0);
		return;
	}
	pattern2_spellcard(bullets, player);
}

// Boss shooting pattern below

void Boss::pattern0(std::vector<Bullet*>& bullets, Player* player) {
	if (!pattern0_initialized) {
		pattern0_lastshootTime_0 = elapsed + 1.0;
		pattern0_lastshootTime_1 = elapsed + 1.0;
		pattern0_initialized = true;
	}

	int player_hitbox_x = player->getX();
	int player_hitbox_y = player->getY();

	if (elapsed >= 0.7 + pattern0_lastshootTime_0) {
		circleroundShoot(bullets, 60, Bullettype::ENEMY_RICE_RD, 3.5);
		pattern0_lastshootTime_0 = elapsed;
		SoundManager::PlaySound("enshoot1", 0, Game::SE_volume / 2);
	}

	if (elapsed >= 3.0 + pattern0_lastshootTime_1) {
		std::vector<double> spd = { 4.0, 4.5, 5.0, 5.5 };
		for (const double& i : spd) {
			aimedcircleroundShoot(bullets, 5, Bullettype::ENEMY_RICE_GR, player_hitbox_x, player_hitbox_y, i);
		}
		pattern0_lastshootTime_1 = elapsed;
	}
}

void Boss::pattern0_spellcard(std::vector<Bullet*>& bullets, Player* player) {
	if (!pattern0_spellcard_initialized) {
		pattern0_spellcard_lastshootTime_total = elapsed;
		pattern0_spellcard_downTime = elapsed + 1.3;
		pattern0_spellcard_lastshootTime_0 = elapsed;
		pattern0_spellcard_reversed = false;
		pattern0_spellcard_shooting = false;
		pattern0_spellcard_initialized = true;
	}

	if (!pattern0_spellcard_shooting && elapsed >= 4.5 + pattern0_spellcard_downTime) {
		pattern0_spellcard_shooting = true;
		pattern0_spellcard_lastshootTime_total = elapsed;
		pattern0_spellcard_downTime = elapsed;
	}

	if (pattern0_spellcard_shooting && elapsed >= 2.0 + pattern0_spellcard_lastshootTime_total) {
		pattern0_spellcard_shooting = false;
	}

	if (elapsed >= 0.05 + pattern0_spellcard_lastshootTime_0 && pattern0_spellcard_shooting) {
		spiralShoot(bullets, 1, Bullettype::ENEMY_PEARL_RD, 3.0, pattern0_spellcard_reversed);
		spiralShoot(bullets, 10, Bullettype::ENEMY_KNIFE, 4.3, pattern0_spellcard_reversed);
		pattern0_spellcard_lastshootTime_0 = elapsed;
		SoundManager::PlaySound("enshoot1", 0, Game::SE_volume / 2);
	}
}

void Boss::pattern1(std::vector<Bullet*>& bullets, Player* player) {
	if (!pattern1_initialized) {
		pattern1_lastshootTime_0 = elapsed + 1.2;
		pattern1_initialized = true;
	}

	int player_hitbox_x = player->getX();
	int player_hitbox_y = player->getY();

	if (elapsed >= 0.4 + pattern1_lastshootTime_0) {
		std::vector<double> spd = { 7.0, 7.5, 8.0, 8.5, 9.0, 9.5, 9.7, 10.0 };
		for (const double& i : spd) {
			aimedcircleroundShoot(bullets, 5, Bullettype::ENEMY_RICE_BL, player_hitbox_x, player_hitbox_y, i);
		}
		pattern1_lastshootTime_0 = elapsed;
		SoundManager::PlaySound("enshoot1", 0, Game::SE_volume / 2);
	}
}

void Boss::pattern1_spellcard(std::vector<Bullet*>& bullets, Player* player) {
	if (!pattern1_spellcard_initialized) {
		pattern1_spellcard_lastshootTime_0 = elapsed;
		pattern1_spellcard_lastshootTime_1 = elapsed;
		pattern1_spellcard_initialized = true;
	}

	int player_hitbox_x = player->getX() - 30;
	int player_hitbox_y = player->getY() - 10;

	if (elapsed >= 4.0 + pattern1_spellcard_lastshootTime_1) {
		std::vector<double> speed = { 5.0, 6.0, 7.0 };
		for (const double& spd : speed) {
			aimedShoot(bullets, Bullettype::ENEMY_PEARL_BL, player_hitbox_x, player_hitbox_y, spd);
		}
		pattern1_spellcard_lastshootTime_1 = elapsed;
		SoundManager::PlaySound("kira0", 0, Game::SE_volume);
	}

	if (elapsed >= 2.0 + pattern1_spellcard_lastshootTime_0) {
		rndShoot(bullets, 25, Bullettype::ENEMY_KUNAI_RD, 3.0);
		rndShoot(bullets, 15, Bullettype::ENEMY_KUNAI_BL, 4.0);
		rndShoot(bullets, 35, Bullettype::ENEMY_KUNAI_GR, 2.0);

		pattern1_spellcard_lastshootTime_0 = elapsed;
		SoundManager::PlaySound("enshoot1", 0, Game::SE_volume / 2);
	}
}

void Boss::pattern2(std::vector<Bullet*>& bullets, Player* player) {
	if (!pattern2_initialized) {
		pattern2_lastshootTime_0 = elapsed - 2.0;
		pattern2_interval = 2.5;
		pattern2_speed = 3.0;
		pattern2_initialized = true;
	}

	if (elapsed >= pattern2_interval + pattern2_lastshootTime_0) {
		rippleShoot(bullets, 70, Bullettype::ENEMY_RICE_BL, pattern2_speed);
		pattern2_lastshootTime_0 = elapsed;
		pattern2_speed += 0.04;
		pattern2_interval -= 0.07;
		if (pattern2_interval <= 0.8) pattern2_interval = 0.8;
		if (pattern2_speed >= 8.0) pattern2_speed = 8.0;
		SoundManager::PlaySound("enshoot1", 0, Game::SE_volume / 2);
	}
}

void Boss::pattern2_spellcard(std::vector<Bullet*>& bullets, Player* player) {
	if (!pattern2_spellcard_initialized) {
		pattern2_spellcard_lastshootTime_0 = elapsed + 0.7;
		pattern2_spellcard_lastshootTime_1 = elapsed + 0.7;
		pattern2_spellcard_lastshootTime_2 = elapsed + 0.7;
		pattern2_spellcard_initialized = true;
	}

	if (elapsed >= 0.7 + pattern2_spellcard_lastshootTime_0) {
		circleroundShoot(bullets, 50, Bullettype::ENEMY_RICE_BL, 2.5);
		pattern2_spellcard_lastshootTime_0 = elapsed;
		SoundManager::PlaySound("enshoot1", 0, Game::SE_volume / 2);
	}

	if (elapsed >= 1.4 + pattern2_spellcard_lastshootTime_1) {
		rippleShoot(bullets, 20, Bullettype::ENEMY_RICE_GR, 3.5);
		pattern2_spellcard_lastshootTime_1 = elapsed;
	}

	if (elapsed >= 0.88 + pattern2_spellcard_lastshootTime_2) {
		circleroundShoot(bullets, 10, Bullettype::ENEMY_RICE_RD, 3.0);
		SoundManager::PlaySound("enshoot1", 0, Game::SE_volume / 2);
		pattern2_spellcard_lastshootTime_2 = elapsed;
	}
}


// Boss shooting functions below

void Boss::aimedShoot(std::vector<Bullet*>& bullets, Bullettype type, int playerX, int playerY, double speed) {
	double deltax = playerX - 35 - destRect.x;
	double deltay = playerY - 35 - destRect.y;

	double angle = atan2(static_cast<long double>(deltay), static_cast<long double>(deltax));

	std::vector<double> buffer = { -M_PI / 6, 0, M_PI / 6 };
	for (const double& offset : buffer) {
		double velx = cos(angle + offset) * speed;
		double vely = sin(angle + offset) * speed;
		bullets.emplace_back(new Bullet(destRect.x + spriteW_center, destRect.y + spriteH_center, velx, vely, type, 0));
	}
}

void Boss::circleroundShoot(std::vector<Bullet*>& bullets, int density, Bullettype type, double speed) {
	double offset = rand() % 50;
	offset /= 10;
	for (int i = 0; i < density; ++i) {
		double angle = (2 * M_PI / density) * i;
		double velx = cos(angle + offset) * speed;
		double vely = sin(angle + offset) * speed;
		bullets.emplace_back(new Bullet(destRect.x + spriteW_center, destRect.y + spriteH_center, velx, vely, type, 0));
	}
}

void Boss::rippleShoot(std::vector<Bullet*>& bullets, int density, Bullettype type, double speed) {
	int randomx = rand() % 500 + 100;
	int randomy = rand() % 150 + 100;

	for (int i = 0; i < density; ++i) {
		double angle = (2 * M_PI / density) * i;
		double velx = cos(angle) * speed;
		double vely = sin(angle) * speed;
		bullets.emplace_back(new Bullet(randomx, randomy, velx, vely, type, 1));
	}
}

void Boss::aimedcircleroundShoot(std::vector<Bullet*>& bullets, int density, Bullettype type, int playerX, int playerY, double speed) {
	double deltax = playerX - 35 - destRect.x;
	double deltay = playerY - 35 - destRect.y;

	double base_angle = atan2(static_cast<long double>(deltay), static_cast<long double>(deltax));
	
	for (int i = 0; i < density; ++i) {
		double angle = base_angle + ((2 * M_PI / density) * i);
		double velx = cos(angle) * speed;
		double vely = sin(angle) * speed;
		bullets.emplace_back(new Bullet(destRect.x + spriteW_center, destRect.y + spriteH_center, velx, vely, type, 0));
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
		bullets.emplace_back(new Bullet(destRect.x + spriteW_center, destRect.y + spriteH_center, velx, vely, type, 0));
		//}
	}
	offset++;
}

void Boss::rndShoot(std::vector<Bullet*>& bullets, int density, Bullettype type, double speed) {
	//int density = 10;

	for (int i = 0; i < density; i++) {
		double angle = (std::rand() % 180) * M_PI / 180;
		double tmpspeed = speed + static_cast<double>((std::rand() % 15)) / 10;
		double velx = cos(angle) * tmpspeed;
		double vely = sin(angle) * tmpspeed;
		bullets.emplace_back(new Bullet(destRect.x + spriteW_center, destRect.y + spriteH_center, velx, vely, type, 0));
	}
}


