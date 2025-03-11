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
class Enemy {
public:
	Enemy(double x, double y, double vx, double vy, EnemyType type);
	~Enemy();
	void update();
	void render();

private:
	double x, y;
	double vx, vy;
	EnemyType type;
	SDL_Texture* texture;
	SDL_Rect srcRect, destRect;

	void BezierCuver();
	void Horizontal();
	void Stationary();
};

#endif 
