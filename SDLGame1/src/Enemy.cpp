#include "headers/Game.hpp"
#include "headers/Bullets.hpp"
#include "headers/Player.hpp"
#include "headers/Enemy.hpp"
#include "headers/SoundManager.hpp"
#include <cmath>
#include <iostream>

Enemy::Enemy(double x, double y, double speed, EnemyType type, MovementType Mtype)
	: xPos(x), yPos(y), type(type), destRect{ 0, 0, 0, 0 }, srcRect{ 0, 0, 0, 0 },
	speed(speed), Enemy_texture(nullptr), Mtype(Mtype), hp(0), vx(0), vy(0), hitbox{ 0, 0, 0, 0 },
	movingHorizontal(true), pause(false), pauseStart(0), useBezier(false), bezierT(0.0), point(0) {

	std::cout << "Enemy spawned at: (" << xPos << ", " << yPos << ")" << std::endl;

	srand(time(0));

	totalFrames = 4;
	Ani_speed = 0.5f;
	frameTime = 0.0f;
	currentFrame = 0;

	initX = static_cast<int>(x); 
	initY = static_cast<int>(y);

	switch (type) {
	case EnemyType::SPARKLE:
		Enemy_texture = Game::Enemy_texture_sparkle;
		spriteW = spriteH = 32;
		hp = 1;
		hitbox = { static_cast<int>(x), static_cast<int>(y), L_HITBOX_SIZE, L_HITBOX_SIZE };
		point = 3000;
		break;
	case EnemyType::RED_FA:
		Enemy_texture = Game::Enemy_texture_r;
		spriteW = 32;
		spriteH = 30;
		hp = 10;
		hitbox = { static_cast<int>(x), static_cast<int>(y), L_HITBOX_SIZE, L_HITBOX_SIZE };
		point = 7000;
		break;
	case EnemyType::WHITE_FA:
		Enemy_texture = Game::Enemy_texture_w;
		spriteW = 32;
		spriteH = 30;
		hp = 60;
		hitbox = { static_cast<int>(x), static_cast<int>(y), L_HITBOX_SIZE, L_HITBOX_SIZE };
		point = 30000;
		break;
	case EnemyType::BLUE_FA:
		Enemy_texture = Game::Enemy_texture_b;
		spriteW = 32;
		spriteH = 30;
		hp = 30;
		hitbox = { static_cast<int>(x), static_cast<int>(y), L_HITBOX_SIZE, L_HITBOX_SIZE };
		point = 25000;
		break;
	}

	srcRect = { 0, 0, spriteW, spriteH };
	destRect = { static_cast<int>(x), static_cast<int>(y), spriteW * 2, spriteH * 2 };

	if (Mtype == MovementType::BezierCurve) {
		useBezier = true;
		if (initX > 420) {
			P0 = Point(initX, initY);
			P1 = Point(550, 250);
			P2 = Point(1510, 1000);
		}
		else {
			P0 = Point( initX, initY );
			P1 = Point(450, 250);
			P2 = Point(-510, 1000);
		}
	} 
}

Enemy::~Enemy() {

}

void Enemy::update() {
	frameTime += Ani_speed;
	if (frameTime >= 1.0f && type != EnemyType::SPARKLE) { // idle animation
		frameTime = 0.0f;
		currentFrame = (currentFrame + 1) % totalFrames;
		srcRect.x = currentFrame * spriteW;
	}

	if (useBezier) {
		BezierCurve();
	}
	else {
		switch (Mtype) {
			case MovementType::Horizontal:
				Horizontal();
				break;
			case MovementType::Vertical:
				Vertical();
				break;
			case MovementType::DiagonalNWSE:
				DiagonalNWSE();
				break;
			case MovementType::DiagonalNESW:
				DiagonalNESW();
				break;
			case MovementType::Lshape:
				Lshape();
				break;

		}
		xPos += vx;
		yPos += vy;
	}
	destRect.x = static_cast<int>(xPos);
	destRect.y = static_cast<int>(yPos);

	hitbox.x = destRect.x + 16;
	hitbox.y = destRect.y + 16;

}

void Enemy::render() {
	if (type == EnemyType::SPARKLE) {
		static int angle = 0;
		angle = (angle + 7 + 360) % 360;
		SDL_RenderCopyEx(Game::Grenderer, Enemy_texture, &srcRect, &destRect, angle, nullptr, SDL_FLIP_NONE);
	}

	else {
		SDL_RenderCopy(Game::Grenderer, Enemy_texture, &srcRect, &destRect);
	}
	//SDL_SetRenderDrawColor(Game::Grenderer, 0, 255, 0, 255); // debug hitbox
	//SDL_RenderFillRect(Game::Grenderer, &hitbox);

}

void Enemy::Horizontal() {
	vx = speed;
	vy = 0.0;
}

void Enemy::BezierCurve() {
	if (bezierT > 1) return;
	else {
		bezierT += 0.01 * speed;
		double u = 1 - bezierT;
	
		xPos = (u * u * P0.x) + (2 * u * bezierT * P1.x) + (bezierT * bezierT * P2.x);
		yPos = (u * u * P0.y) + (2 * u * bezierT * P1.y) + (bezierT * bezierT * P2.y);
	}
}

void Enemy::Lshape() {
	speed = std::abs(speed);

	if (movingHorizontal) {
		if (xPos < ((PLAY_AREA_X_MAX - PLAY_AREA_X_MIN) / 2) + PLAY_AREA_X_MIN) {
			vx = speed;
		}
		else {
			vx = -speed;
		}
		vy = 0.0;

		if (std::abs(xPos - initX) >= 300) {
			movingHorizontal = false;
			pause = true;
			pauseStart = SDL_GetTicks();
			vx = 0.0;
			vy = 0.0;
		}
	}
	else if (pause) {
		if (SDL_GetTicks() - pauseStart >= 1000) {
			pause = false;
		}
	}
	else {
		vy = -speed;
	}
}

void Enemy::DiagonalNWSE() {
	vx = speed;
	vy = speed * 0.5;
}

void Enemy::DiagonalNESW() {
	vx = speed;
	vy = -speed * 0.5;
}


void Enemy::Vertical() {
	//static double n = 0.0;
	//n += 0.01;
	vx = 0.0;
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
	SoundManager::PlaySound("enshoot0", 0, 16);
}



void Enemy::aimedShoot(std::vector<Bullet*>& bullets, int playerX, int playerY) {
	double deltax = playerX - destRect.x; 
	double deltay = playerY - destRect.y; 

	double angle = atan2((long double) deltay,(long double) deltax);

	std::vector<double> buffer = { -M_PI / 6, 0, M_PI / 6 };
	std::vector<double> spdvar = { 3.6, 4, 4.2, 4.5, 5, 6, 7 ,8 };
	for (double offset : buffer) {
		for (double speed : spdvar) {
			double velx = cos(angle + offset) * speed;
			double vely = sin(angle +offset) * speed;
			bullets.emplace_back(new Bullet(destRect.x, destRect.y, velx, vely, Bullettype::ENEMY_KUNAI_BL));
			
		}
	}
	SoundManager::PlaySound("enshoot1", 0, 16);
	fired = true;
}

void Enemy::circleroundShoot(std::vector<Bullet*>& bullets, int density) {
	//int m = 10;
	for (int i = 0; i < density; ++i) {
		double angle = (2 * M_PI / density) * i;
		std::vector<double> spdvar = { 3 , 3.5};
		for (double speed : spdvar) {
			double velx = cos(angle) * speed;
			double vely = sin(angle) * speed;
			bullets.emplace_back(new Bullet(destRect.x, destRect.y, velx, vely, Bullettype::ENEMY_ROUND1));
		}
	}
	SoundManager::PlaySound("enshoot2", 0, 16);
	fired = true;
}

void Enemy::deathShoot(std::vector<Bullet*>& bullets, int density) {
	for (int i = 0; i < density; ++i) {
		double randomOffset = (rand() % 360) * (M_PI / 180.0);
		double angle = ((2 * M_PI / density) * i) + randomOffset;
		double velx = cos(angle) * 3;
		double vely = sin(angle) * 3;
		bullets.emplace_back(new Bullet(destRect.x, destRect.y, velx, vely, Bullettype::ENEMY_KUNAI_GR));
	}
}
