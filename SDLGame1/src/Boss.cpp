#include "headers/Boss.hpp"
#include "headers/Bullets.hpp"
#include "headers/Player.hpp"
#include "headers/Items.hpp"
#include "headers/Game.hpp"


Boss::Boss(double x, double y)
	: xPos(x), yPos(y), destRect{ 0, 0, 0, 0 }, srcRect{ 0, 0, 0, 0 }, isIdle(true),
	hp(0), vx(0), vy(0), hitbox{ 0, 0, 0, 0 }, spellcard_isactive(false), phase(Phase::IDLE) {

	Boss_texture = TextureManager::LoadTexture("res/Enemy/Flandre.png");

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
	destRect = { static_cast<int>(x), static_cast<int>(y), spriteW * 2, spriteH * 2 };
}

Boss::~Boss() {
	if (Boss_texture) {
		SDL_DestroyTexture(Boss_texture);
		Boss_texture = nullptr;
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

void Boss::moveinscreen() {
	/*std::cout << "movement(): called" << std::endl;*/
	if (xPos >= 410 && yPos == 120) {
		phase = Phase::PHASE0;
		vx = 0;
		vy = 0;
		xPos = 410;
		yPos = 120;
	}
	else {
		vx = 20;
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
}

Phase Boss::getPhase() const {
	return phase;
}
