#ifndef ENEMY_HPP
#define ENEMY_HPP

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <vector>
#include <random>


class Bullet;
class Player;
class Game;


enum class EnemyType {
	RED_FA,
	BLUE_FA, // types
	WHITE_FA,
	SPARKLE,
};

enum class MovementType {
	Horizontal,
	Vertical, // Mtypes
	BezierCurve,
	DiagonalNWSE,
	DiagonalNESW,
	Lshape,
};

class Enemy {
public:
	Enemy(double x, double y, double speed, EnemyType type, MovementType Mtype);
	~Enemy();
	void update();
	void render();
	void rndriceShoot(std::vector<Bullet*>& bullets, int density);
	void aimedShoot(std::vector<Bullet*>& bullets, int x ,int y);
	void circleroundShoot(std::vector<Bullet*>& bullets, int density);

	int initX; // store where the enemy was spawned
	int initY;
	int getY() const;
	int getX() const;
	int getEnemyhp() const;
	void updatehp(int in_hp);
	SDL_Rect getEnHitbox() const;
	EnemyType getType() const;

	bool fired = false;

private:
	const int L_HITBOX_SIZE = 20; // hitbox size
	double vx, vy; // velocity
	double xPos, yPos; // position in cartesian plane
	double speed; // speed
	int hp; // hp
	EnemyType type; // enemy type
	MovementType Mtype; // movement type
	SDL_Texture* Enemy_texture;
	SDL_Rect srcRect, destRect;
	SDL_Rect hitbox;


	bool movingHorizontal;
	bool pause;
	Uint32 pauseStart;

	/*void BezierCurve();
	double newX, newY;*/

	double movementProgress = 0.0;

	void Horizontal();
	void Vertical();
	void DiagonalNWSE();
	void DiagonalNESW();
	void Lshape();

	int currentFrame;
	int totalFrames;
	float Ani_speed;
	float frameTime;

	int spriteW = 0, spriteH = 0;
};

#endif 
