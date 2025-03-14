#include "headers/Game.hpp"
#include "headers/Bullets.hpp"
#include "headers/Player.hpp"
#include "headers/Enemy.hpp"
#include <cmath>

Enemy::Enemy(double x, double y, double speed, EnemyType type, MovementType Mtype)
	: xPos(x) ,yPos(y), type(type), destRect{ 0, 0, 0, 0 }, srcRect{ 0, 0, 0, 0 },
	speed(speed), Enemy_texture(nullptr), Mtype(Mtype){
	totalFrames = 4;
	Ani_speed = 0.5f;
	frameTime = 0.0f;
	currentFrame = 0;


	switch (type) {
		case EnemyType::RED_FA:
			Enemy_texture = TextureManager::LoadTexture("res/Enemy/Red_fa.png");
			spriteW = 32;
			spriteH = 30;
			break;
		case EnemyType::WHITE_FA:
			Enemy_texture = TextureManager::LoadTexture("res/Enemy/White_fa.png");
			spriteW = 32;
			spriteH = 30;
			break;
	}

	srcRect = { 0, 0, spriteW, spriteH };
	destRect = { (int)x, (int)y, spriteW * 2, spriteH * 2 };
}

Enemy::~Enemy() {
	SDL_DestroyTexture(Enemy_texture);
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
			break;
		case MovementType::Vertical:
			Vertical();
			break;
	}
	xPos += vx;
	yPos += vy;
	destRect.x = static_cast<int>(xPos);
	destRect.y = static_cast<int>(yPos);
}

void Enemy::render() {
	SDL_RenderCopy(Game::Grenderer, Enemy_texture, &srcRect, &destRect);
}

void Enemy::Horizontal() {
	vx = speed;
	vy = 0.5;
}

//void Enemy::BezierCurve() {
//
//}

void Enemy::Vertical() {
	vx = 0;
	vy = speed;
}

void Enemy::enemyShoot(std::vector<Bullet*>& bullets) {
	bullets.push_back(new Bullet(destRect.x, destRect.y, 0, 4, Bullettype::ENEMY_KUNAI));
	fired = true;
}

void Enemy::testShoot(std::vector<Bullet*>& bullets, int playerX, int playerY) {
	double deltax = playerX - destRect.x; 
	double deltay = playerY - destRect.y; 

	double angle = atan2((long double) deltay,(long double) deltax);

	std::vector<double> buffer = { -M_PI / 6, 0, M_PI / 6 };
	std::vector<double> spdvar = { 7 ,9, 12 };
	//for (double offset : buffer) {
		for (double speed : spdvar) {
			double velx = cos(angle ) * speed;
			double vely = sin(angle ) * speed;
			bullets.emplace_back(new Bullet(destRect.x, destRect.y, velx, vely, Bullettype::ENEMY_KUNAI));
		}
	//}
	fired = true;
}

//void Player::testshoot(std::vector<Bullet*>& bullets) {
//    int x = 80;
//    int y = 100;
//	double vx = 0;
//    double vy = 0;
//    double deltax = destRect.x - x;
//	double deltay = destRect.y - y;
//	double angle = atan2(deltay, deltax);
//	std::vector<double> buffer = { 3.1412 / 6, 0, -3.1412 / 6 };
//	for (auto offset : buffer) {
//		vx = cos(angle + offset) * 30.0;
//		vy = sin(angle + offset) * 30.0;
//		bullets.push_back(new Bullet(Game::Grenderer, x, y, vx, vy, Bullettype::PLAYER_0));
//	}
//	
//
//}

int Enemy::getX() const {
	return destRect.x;
}

int Enemy::getY() const {
	return destRect.y;
}

