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
	BLUE_FA,
	WHITE_FA,
};

enum class MovementType {
	Horizontal,
	Stationary,
	BezierCurve,
};

class Enemy {
public:
	Enemy(double x, double y, double speed, EnemyType type, MovementType Mtype);
	~Enemy();
	void update();
	void render();

	int getY() const;
	int getX() const;

private:
	double vx, vy;
	double speed;
	EnemyType type;
	MovementType Mtype;
	SDL_Texture* Enemy_texture;
	SDL_Rect srcRect, destRect;


	void BezierCurve();
	void Horizontal();
	void Stationary();


	int currentFrame;
	int totalFrames;
	float Ani_speed;
	float frameTime;

	int spriteW = 0, spriteH = 0;
};

#endif 
