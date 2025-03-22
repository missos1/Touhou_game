#ifndef COLLISION_HPP
#define COLLISION_HPP

#include <SDL.h>
#include <iostream>
#include <vector>

class Game;
class Bullet;
class Player;
class Enemy;
class SoundManager;

class CollisionCheck {
public:
	static void PlayerColli(std::vector<Bullet*>& bullets, Player* player);
	static void EnemyColli(std::vector<Bullet*>& bullets, std::vector<Enemy*>& enemies);
	//static void BossColli(std::vector<Bullet*>& bullets,);
};

#endif 
