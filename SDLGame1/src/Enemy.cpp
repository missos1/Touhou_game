#include "headers/Game.hpp"
#include "headers/Bullets.hpp"
#include "headers/Player.hpp"
#include "headers/Enemy.hpp"
#include "headers/SoundManager.hpp"
#include <cmath>

Enemy::Enemy(double x, double y, double speed, EnemyType type, MovementType Mtype)
	: xPos(x), yPos(y), type(type), destRect{ 0, 0, 0, 0 }, srcRect{ 0, 0, 0, 0 },
	speed(speed), Enemy_texture(nullptr), Mtype(Mtype), hp(0), vx(0), vy(0), hitbox{ 0, 0, 0, 0 } {
	totalFrames = 4;
	Ani_speed = 0.5f;
	frameTime = 0.0f;
	currentFrame = 0;


	switch (type) {
	case EnemyType::RED_FA:
		Enemy_texture = Game::Enemy_texture_r;
		spriteW = 32;
		spriteH = 30;
		hp = 3;
		hitbox = { (int)x, (int)y, L_HITBOX_SIZE, L_HITBOX_SIZE };
		break;
	case EnemyType::WHITE_FA:
		Enemy_texture = Game::Enemy_texture_w;
		spriteW = 32;
		spriteH = 30;
		hp = 10;
		hitbox = { (int)x, (int)y, L_HITBOX_SIZE, L_HITBOX_SIZE };
		break;
	case EnemyType::BLUE_FA:
		Enemy_texture = Game::Enemy_texture_b;
		spriteW = 32;
		spriteH = 30;
		hp = 8;
		hitbox = { (int)x, (int)y, L_HITBOX_SIZE, L_HITBOX_SIZE };
		break;
	}

	srcRect = { 0, 0, spriteW, spriteH };
	destRect = { (int)x, (int)y, spriteW * 2, spriteH * 2 };
}

Enemy::~Enemy() {

}

void Enemy::update() {
	frameTime += Ani_speed;
	if (frameTime >= 1.0f) { // idle animation
		frameTime = 0.0f;
		currentFrame = (currentFrame + 1) % totalFrames;
		srcRect.x = currentFrame * spriteW;
	}

	switch (Mtype) {
		case MovementType::Horizontal:
			Horizontal();
			xPos += vx;
			yPos += vy;
			break;
		case MovementType::Vertical:
			Vertical();
			xPos += vx;
			yPos += vy;
			break;
	}
	xPos += vx;
	yPos += vy;
	destRect.x = static_cast<int>(xPos);
	destRect.y = static_cast<int>(yPos);

	hitbox.x = destRect.x + 16;
	hitbox.y = destRect.y + 16;
}

void Enemy::render() {
	SDL_RenderCopy(Game::Grenderer, Enemy_texture, &srcRect, &destRect);

	SDL_SetRenderDrawColor(Game::Grenderer, 0, 255, 0, 255); // debug hitbox
	SDL_RenderFillRect(Game::Grenderer, &hitbox);
}

void Enemy::Horizontal() {
	vx = speed;
	vy = -0.5;

}

//void Enemy::BezierCurve() {
//
//}

void Enemy::Vertical() {
	//static double n = 0.0;
	//n += 0.01;
	vx = 0;
	vy = speed;
}

void Enemy::rndriceShoot(std::vector<Bullet*>& bullets, int density) {
	//int density = 10;
	srand(SDL_GetTicks());
	for (int i = 0; i < density; i++) {
		double angle = (std::rand() % 180) * M_PI / 180;
		double speed = 3 + std::rand() % 2;
		double velx = cos(angle) * speed;
		double vely = sin(angle) * speed;
		bullets.emplace_back(new Bullet(destRect.x, destRect.y, velx, vely, Bullettype::ENEMY_RICE));
	}
	SoundManager::PlaySound("enshoot0", 0, 32);
	fired = true;
}

void Enemy::aimedShoot(std::vector<Bullet*>& bullets, int playerX, int playerY) {
	double deltax = playerX - destRect.x; 
	double deltay = playerY - destRect.y; 

	double angle = atan2((long double) deltay,(long double) deltax);

	std::vector<double> buffer = { -M_PI / 6, 0, M_PI / 6 };
	std::vector<double> spdvar = { 3.6, 4, 4.2, 4.5, 5, 6 };
	for (double offset : buffer) {
		for (double speed : spdvar) {
			double velx = cos(angle + offset) * speed;
			double vely = sin(angle +offset) * speed;
			bullets.emplace_back(new Bullet(destRect.x, destRect.y, velx, vely, Bullettype::ENEMY_KUNAI_BL));
			
		}
	}
	SoundManager::PlaySound("enshoot0", 0, 32);
	fired = true;
}

void Enemy::circleroundShoot(std::vector<Bullet*>& bullets, int density) {
	//int m = 10;
	for (int i = 0; i < density; ++i) {
		double angle = (2 * M_PI / density) * i;
		std::vector<double> spdvar = { 2.5, 2.3, 2,  };
		for (double speed : spdvar) {
			double velx = cos(angle) * speed;
			double vely = sin(angle) * speed;
			bullets.emplace_back(new Bullet(destRect.x, destRect.y, velx, vely, Bullettype::ENEMY_ROUND1));
			SoundManager::PlaySound("enshoot1", 0, 32);
		}
	}
	fired = true;
}

int Enemy::getX() const {
	return destRect.x;
}

int Enemy::getY() const {
	return destRect.y;
}

EnemyType Enemy::getType() const{
	return type;
}

SDL_Rect Enemy::getEnHitbox() const {
	return hitbox;
}

int Enemy::getEnemyhp() const {
	return hp;
}

void Enemy::updatehp(int in_hp) {
	hp = in_hp;
}