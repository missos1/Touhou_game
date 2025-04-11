#ifndef BOSS_HPP
#define BOSS_HPP

#include <SDL.h>
#include <SDL_ttf.h>
#include <vector>
#include <random>
#include <ctime>
#include <iostream>
#include "TextureManager.hpp"
#include "SoundManager.hpp"

class Game;
class Player;
class Bullet;
enum class Bullettype;
class Item;

enum class Phase {
	IDLE,
	PHASE0,
	PHASE0_SC,
	PHASE1,
	PHASE1_SC,
};

enum class BossState {
	NORMAL,
	RETURNING,
};

class Boss {
public: 
	Boss(double x, double y);
	~Boss(); 

	void update();
	void render();
	void resetValue();

	void debug_ani(const Uint8* keys);
	void moveinscreen();
	void move_returning();
	void phase0(std::vector<Bullet*>& bullets, Player* player);
	void phase0_spellcard(std::vector<Bullet*>& bullets, Player* player);
	void phase1(std::vector<Bullet*>& bullets, Player* player);

	void pattern0(std::vector<Bullet*>& bullets, Player* player);
	void pattern0_spellcard(std::vector<Bullet*>& bullets, Player* player);
	void pattern1(std::vector<Bullet*>& bullets, Player* player);
	void pattern1_spellcard(std::vector<Bullet*>& bullets, Player* player);

	void aimedShoot(std::vector<Bullet*>& bullets, Bullettype type, int playerX, int playerY, double speed);
	void circleroundShoot(std::vector<Bullet*>& bullets, int density, Bullettype type, double speed);
	void aimedcircleroundShoot(std::vector<Bullet*>& bullets, int density, Bullettype type, int playerX, int playerY, double speed);
	void rndShoot(std::vector<Bullet*>& bullets, int density, Bullettype type, double speed);

	int getX() const { return destRect.x; }
	int getY() const { return destRect.y; }
	SDL_Rect getBossHitbox() const { return hitbox; }
	int getBosshp() const { return hp; }
	void updatehp(int in_hp) { hp = in_hp; }
	int getPoint() const { return point; }
	Phase getPhase() const;

private:
	double vx, vy; // velocity
	double xPos, yPos; // position in cartesian plane
	Phase phase;
	BossState state;
	int hp;// hp
	int point;

	SDL_Texture* Boss_texture;
	SDL_Texture* Circle_texture;
	SDL_Texture* Spellcard_texture;
	SDL_Rect srcRect, destRect;
	SDL_Rect srcRect_circle, destRect_circle;
	SDL_Rect hitbox;

	bool spellcard_isactive;
	bool isIdle;
	bool isMoving;
	bool isFlipped;

	int currentFrame;
	int currentFrameIdle;
	int totalFrames;
	float Ani_speed;
	float frameTime;

	const int spriteW = 64, spriteH = 74;
	const int spriteW_center = spriteW / 2;
	const int spriteH_center = spriteH / 2;
};

#endif // !BOSS_HPP
