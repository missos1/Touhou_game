#ifndef COLLISION_HPP
#define COLLISION_HPP

#include <SDL.h>
#include <iostream>
#include <vector>
#include <random>
#include <ctime>

class Game;
class Bullet;
class Player;
class Enemy;
class SoundManager;
class Item;

class CollisionCheck {
public:
	static void PlayerColli(std::vector<Bullet*>& bullets, Player* player, std::vector<Item*>& items);
	static void EnemyColli(std::vector<Bullet*>& bullets, std::vector<Enemy*>& enemies, std::vector<Item*>& items, Player* player);
	//static void BossColli(std::vector<Bullet*>& bullets,);
};

#endif 
