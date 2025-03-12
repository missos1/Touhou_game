#include "headers/Game.hpp"
#include "headers/Bullets.hpp"
#include "headers/Player.hpp"
#include "headers/Enemy.hpp"

Enemy::Enemy(double x, double y, double speed, EnemyType type, MovementType Mtype)
	: vx(0), vy(0), type(type), destRect{ 0, 0, 0, 0 }, srcRect{ 0, 0, 0, 0 },
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
	}
	destRect.x += vx;
	destRect.y += vy;
}

void Enemy::render() {
	SDL_RenderCopy(Game::Grenderer, Enemy_texture, &srcRect, &destRect);
}

void Enemy::Horizontal() {
	vx = speed;
	vy = 0;
}

int Enemy::getX() const {
	return destRect.x;
}

int Enemy::getY() const {
	return destRect.y;
}

