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
	PHASE2,
	PHASE2_SC,
	DEAD,
};

enum class BossState {
	NORMAL,
	RETURNING,
};

class Boss {
public: 
	Boss(double x, double y);
	~Boss(); 

	void update(std::vector<Bullet*>& bullets, std::vector<Item*>& items);
	void render();
	void resetValue();

	void debug_ani(const Uint8* keys);
	void moveinscreen();
	void phase0(std::vector<Bullet*>& bullets, Player* player);
	void phase0_spellcard(std::vector<Bullet*>& bullets, Player* player);
	void phase1(std::vector<Bullet*>& bullets, Player* player);
	void phase1_spellcard(std::vector<Bullet*>& bullets, Player* player);
	void phase2(std::vector<Bullet*>& bullets, Player* player);
	void phase2_spellcard(std::vector<Bullet*>& bullets, Player* player);

	int getX() const { return destRect.x; }
	int getY() const { return destRect.y; }
	SDL_Rect getBossHitbox() const { return hitbox; }
	int getBosshp() const { return currentHP; }
	double getBosshp_percent() { return static_cast<double>(currentHP) / phaseHP[phase]; }
	int getBoss_spellcardcount() const { return spellcard_count; }
	int getTimeleft() const { return phase_time_left; }
	bool isphase_spellcard() const { return (phase == Phase::PHASE0_SC || phase == Phase::PHASE1_SC || phase == Phase::PHASE2_SC); }

	Phase getPhase() const { return phase; };
	void checkforbonus(Player* player);
	void takeDamage(int damage);
	void clearScreen(std::vector<Bullet*>& bullets, std::vector<Item*>& items);
	void getStage_elapsed(const double& elapsed);

private:
	void animation();

	void movement_returning(double targetX, double targetY, double speed);
	void movement_random(double speed, Uint64 idle_cooldown);

	void pattern0(std::vector<Bullet*>& bullets, Player* player);
	void pattern0_spellcard(std::vector<Bullet*>& bullets, Player* player);
	void pattern1(std::vector<Bullet*>& bullets, Player* player);
	void pattern1_spellcard(std::vector<Bullet*>& bullets, Player* player);
	void pattern2(std::vector<Bullet*>& bullets, Player* player);
	void pattern2_spellcard(std::vector<Bullet*>& bullets, Player* player);

	void aimedShoot(std::vector<Bullet*>& bullets, Bullettype type, int playerX, int playerY, double speed);
	void circleroundShoot(std::vector<Bullet*>& bullets, int density, Bullettype type, double speed);
	void aimedcircleroundShoot(std::vector<Bullet*>& bullets, int density, Bullettype type, int playerX, int playerY, double speed);
	void rndShoot(std::vector<Bullet*>& bullets, int density, Bullettype type, double speed);
	void spiralShoot(std::vector<Bullet*>& bullets, int density, Bullettype type, double speed, bool Reversed);
	void rippleShoot(std::vector<Bullet*>& bullets, int density, Bullettype type, double speed);
	void updatePhase(std::vector<Bullet*>& bullets, std::vector<Item*>& items);
	void setPhase(Phase newPhase); 

	double vx, vy; // velocity
	double xPos, yPos; // position in cartesian 

	Phase phase;
	BossState state;

	int phase_duration;
	int phase_time_left;
	std::unordered_map<Phase, int> phase_time_map; // Time for each phase
	int elapsed;

	int currentHP; // Current HP of the boss
	std::unordered_map<Phase, int> phaseHP; // HP for each phase
	int spellcard_count;
	
	SDL_Texture* Boss_texture;
	SDL_Texture* Circle_texture;
	SDL_Texture* Spellcard_texture;
	SDL_Texture* text_texture;
	SDL_Rect srcRect, destRect;
	SDL_Rect srcRect_circle, destRect_circle;
	SDL_Rect destRect_spellcard;
	SDL_Rect hitbox;

	bool isIdle;
	bool isMoving;
	bool isFlipped;
	bool Bonus;

	int playerhp_track;

	int currentFrame;
	int currentFrameIdle;
	int totalFrames; 
	float Ani_speed;
	float frameTime;

	const int spriteW = 64, spriteH = 74;
	const int spriteW_center = spriteW / 2;
	const int spriteH_center = spriteH / 2;
	const int BOSS_DEFAULT_X = 410;
	const int BOSS_DEFAULT_Y = 180;
	const int BOSS_HP_WITDH_0 = 128;
	const int BOSS_HP_WITDH_1 = 256;
};

#endif // !BOSS_HPP
