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
class Boss;


class CollisionCheck {
public:
	static void PlayerColli(std::vector<Bullet*>& bullets, Player* player, std::vector<Item*>& items);
	static void EnemyColli(std::vector<Bullet*>& bullets, std::vector<Enemy*>& enemies, std::vector<Item*>& items, Player* player);
	static void BossColli(std::vector<Bullet*>& player_bullets, Boss* boss, std::vector<Item*>& items);
	//static void EnemyCalculation();
	static void DeleleOffScreen(std::vector<Bullet*>& bullets, std::vector<Bullet*>& player_bullets, std::vector<Enemy*>& enemies, std::vector<Item*>& items);
private:
	static void ItemGetCalculation(std::vector<Item*>& items, Player* player);
};

#endif 
