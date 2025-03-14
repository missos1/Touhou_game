#ifndef ENEMY_HPP
#define ENEMY_HPP

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <vector>

class Bullet;
class Player;
class Game;

enum class EnemyType {
	RED_FA,
	BLUE_FA, // types
	WHITE_FA,
};

enum class MovementType {
	Horizontal,
	Vertical, // Mtypes
	BezierCurve,
};

class Enemy {
public:
	Enemy(double x, double y, double speed, EnemyType type, MovementType Mtype);
	~Enemy();
	void update();
	void render();
	void enemyShoot(std::vector<Bullet*>& bullets);
	void testShoot(std::vector<Bullet*>& bullets, int x ,int y);

	int getY() const;
	int getX() const;
	bool fired = false;

private:
	double vx, vy;
	double xPos, yPos;
	double speed;
	EnemyType type;
	MovementType Mtype;
	SDL_Texture* Enemy_texture;
	SDL_Rect srcRect, destRect;

	/*void BezierCurve();
	double newX, newY;*/

	double movementProgress = 0.0;

	void Horizontal();
	void Vertical();

	int currentFrame;
	int totalFrames;
	float Ani_speed;
	float frameTime;

	int spriteW = 0, spriteH = 0;
};

#endif 
