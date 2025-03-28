#ifndef BOSS_HPP
#define BOSS_HPP

#include <SDL.h>
#include <SDL_ttf.h>
#include <vector>
#include <random>
#include <ctime>
#include "TextureManager.hpp"
#include "SoundManager.hpp"

class Game;
class Player;
class Bullet;
class Item;

enum class Phase {
	IDLE,
	PHASE0,
	PHASE0_SC,
	PHASE1,
	PHASE1_SC,
};

class Boss {
public: 
	Boss(double x, double y);
	~Boss(); 

	void update();
	void render();

	void debug_ani(const Uint8* keys);

	void pattern0(std::vector<Bullet*>& bullets);
	void pattern0_spellcard(std::vector<Bullet*>& bullets);
	void pattern1(std::vector<Bullet*>& bullets);
	void pattern1_spellcard(std::vector<Bullet*>& bullets);

	int getX() const { return destRect.x; }
	int getY() const { return destRect.y; }
	SDL_Rect getBossHitbox() const { return hitbox; }
	int getBosshp() const { return hp; }
	void updatehp(int in_hp) { hp = in_hp; }
	int getPoint() const { return point; }
	int getHPperphase() const;

private:
	double vx, vy; // velocity
	double xPos, yPos; // position in cartesian plane
	double speed; // speed
	Phase phase;
	int hp;// hp
	int point;



	SDL_Texture* Boss_texture;
	SDL_Texture* Boss_move_texture;
	SDL_Texture* Spellcard_texture;
	SDL_Rect srcRect, destRect;
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
};

#endif // !BOSS_HPP
