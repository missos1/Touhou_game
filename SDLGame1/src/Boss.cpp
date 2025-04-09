#include "headers/Boss.hpp"
#include "headers/Bullets.hpp"
#include "headers/Player.hpp"
#include "headers/Items.hpp"
#include "headers/Game.hpp"


Boss::Boss(double x, double y)
	: xPos(x), yPos(y), destRect{ 0, 0, 0, 0 }, srcRect{ 0, 0, 0, 0 }, isIdle(true),
	hp(0), vx(0), vy(0), hitbox{ 0, 0, 0, 0 }, spellcard_isactive(false), phase(Phase::IDLE), state(BossState::NORMAL) {

	Boss_texture = TextureManager::LoadTexture("res/Enemy/Flandre.png");
	Circle_texture = TextureManager::LoadTexture("res/Enemy/circle.png");
	Spellcard_texture = TextureManager::LoadTexture("res/Enemy/Flandre_SpellCard.png");

	totalFrames = 4;
	Ani_speed = 0.1f;
	frameTime = 0.0f;
	currentFrame = 0;
	currentFrameIdle = 0;

	switch (phase) {
	case Phase::IDLE:
		break;
	case Phase::PHASE0:
		hp = 5000;
		point = 10000;
		break;
	case Phase::PHASE0_SC:
		hp = 7000;
		point = 100000;
		break;
	case Phase::PHASE1:
		hp = 6000;
		point = 20000;
		break;
	case Phase::PHASE1_SC:
		hp = 10000;
		point = 150000;
		break;
	}
	srcRect = { 0, 0, spriteW, spriteH };
	srcRect_circle = { 0, 0, 60, 59 };
	destRect = { static_cast<int>(x), static_cast<int>(y), spriteW * 2, spriteH * 2 };
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

void Boss::update() {
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

	hitbox = destRect;
}

void Boss::render() {
	
	SDL_RenderCopyEx(Game::Grenderer, Boss_texture, &srcRect, &destRect, 0, nullptr, isFlipped ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL);

	static int angle = 0;
	angle = (angle + 1 + 360) % 360;
	destRect_circle = { destRect.x - 50, destRect.y - 40, 60 * 4, 59 * 4 };
	SDL_RenderCopyEx(Game::Grenderer, Circle_texture, &srcRect_circle, &destRect_circle, angle, nullptr, SDL_FLIP_NONE);
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
	//std::cout << "movement(): called" << std::endl;

	if (xPos >= 410 && yPos == 120) {
		phase = Phase::PHASE0;

		vx = 0;
		vy = 0;
		xPos = 410;
		yPos = 120;
	}
	else {
		vx = 10;
		//std::cout << "speed" << vx << std::endl;
	}
}

void Boss::phase0(std::vector<Bullet*>& bullets, Player* player) {
	const double leftLimit = 200;
	const double rightLimit = 600;
	const double speed = 4.0;

	static Uint32 stationaryTime = 0;
	Uint32 currentTime = SDL_GetTicks();

	static bool stationary = true;
	static bool movingRight = false;
	static bool justspawned = true;

	if (justspawned) {
		justspawned = false;
		stationaryTime = currentTime;
	}

	// Move left and right between leftLimit and rightLimit
	if (stationary) {
		vx = 0;
		vy = 0;
		if (currentTime - stationaryTime >= 3000) {
			stationary = false;
		}
	}
	else if (movingRight) {
		vx = speed;
		if (xPos >= rightLimit) {
			movingRight = false;
			stationary = true;
			stationaryTime = currentTime;
		}
	}
	else {
		vx = -speed;
		if (xPos <= leftLimit) {
			movingRight = true;
			stationary = true;
			stationaryTime = currentTime;
		}
	}

	vy = 0; // stay at the same Y position

	pattern0(bullets, player);
}

void Boss::phase0_spellcard(std::vector<Bullet*>& bullets, Player* player) {
	
}



// Boss shooting pattern below

void Boss::pattern0(std::vector<Bullet*>& bullets, Player* player) {
	//static bool reversed = false;
	static Uint32 lastshootTime_0 = 0;
	static Uint32 lastshootTime_1 = 0;
	static Uint32 lastshootTime_2 = 0;
	Uint32 currentTime = SDL_GetTicks();

	if (!isIdle) {
		if (currentTime - lastshootTime_0 >= 400) {
			circleroundShoot(bullets, 15, Bullettype::ENEMY_RICE_RD, 4);
			circleroundShoot(bullets, 20, Bullettype::ENEMY_RICE_BL, 3.5);
			circleroundShoot(bullets, 38, Bullettype::ENEMY_RICE_GR, 3.3);
			lastshootTime_0 = currentTime;
			SoundManager::PlaySound("enshoot1", 0, 255);
		}

		//if (currentTime - lastshootTime_1 >= 600) {
		//	//aimedcircleroundShoot(bullets, 40, Bullettype::ENEMY_KNIFE, player->getX(), player->getY(), 5);
		//	lastshootTime_1 = currentTime;
		//}
	}
	else {
		if (currentTime - lastshootTime_2 >= 500) {
			//aimedShoot(bullets, Bullettype::ENEMY_RICE, player->getX(), player->getY(), 6);
			rndShoot(bullets, 10, Bullettype::ENEMY_KNIFE, 5);
			lastshootTime_2 = currentTime;
		}
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
	for (int i = 0; i < density; ++i) {
		double angle = (2 * M_PI / density) * i;
		//std::vector<double> spdvar = { 5 };
		//for (double speed : spdvar) {
			double velx = cos(angle) * speed;
			double vely = sin(angle) * speed;
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


Phase Boss::getPhase() const {
	return phase;
}
