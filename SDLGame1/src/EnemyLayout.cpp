#include "headers/Enemy.hpp"
#include "headers/Player.hpp"
#include "headers/Game.hpp"
#include "headers/Bullets.hpp"
#include "headers/EnemyLayout.hpp"
#include <iostream>
#include <cmath>
#define endl "\n"

void EnemyLayout::wave1(std::vector<Enemy*>& enemies, std::vector<Bullet*>& bullets, Player*& player) {
	static double lastexecuteTime = -1.0;

	const double initTime = 0.0;
	double tmp = SDL_GetTicks() / 1000.0;
	double elapsed = SDL_GetTicks() / 1000.0 - initTime;

	//std::cout << "initTime: " << initTime << endl;

	elapsed = round(elapsed * 10) / 10;
	tmp = round(tmp * 10) / 10;
	//std::cout << "realtime: " << tmp << endl;

	if (elapsed < 0.0) return;
	//std::cout << "elapsed: " << elapsed << endl;


	if (elapsed != lastexecuteTime) {

		lastexecuteTime = elapsed;
		
		double loop = 5;

		double interval = 0.4;

		std::vector<double> spawntime((int)loop);

		for (int i = 1; i <= (int)loop - 1; i++) {
			spawntime.emplace_back(interval * i);
		}

		for (auto spawn : spawntime) {
			if (elapsed == 1 + spawn) {
				enemies.emplace_back(new Enemy(-30, 300, 4, EnemyType::RED_FA, MovementType::Horizontal));
				std::cout << "Spawning enemies!" << endl;
			}
			if (elapsed == 4 + spawn) {
				enemies.emplace_back(new Enemy(800, 200, -4, EnemyType::RED_FA, MovementType::Horizontal));
				std::cout << "Spawning enemies!" << endl;
			}
			if (elapsed == 10) {
				enemies.emplace_back(new Enemy(600, -30, 4, EnemyType::WHITE_FA, MovementType::Vertical));
				enemies.emplace_back(new Enemy(300, -30, 4, EnemyType::WHITE_FA, MovementType::Vertical));
				std::cout << "Spawning enemies!" << endl;
			}
		}
	}

	Uint32 currentTime = SDL_GetTicks();

	static std::unordered_map<Enemy*, Uint32> enemyLastShootTime; // storing timimg for each enemies

	for (int i = (int)enemies.size() - 1; i >= 0; i--) {
		/*if (enemies[i]->getX() >= 350 && enemies[i]->getX() <= 550)*/
		switch (enemies[i]->getType()) {
		case EnemyType::RED_FA:
			if (currentTime - enemyLastShootTime[enemies[i]] > 400) {
				enemyLastShootTime[enemies[i]] = currentTime;
				enemies[i]->rndriceShoot(bullets);
			}
			break;
		case EnemyType::WHITE_FA:
			if (enemies[i]->getY() >= 100 && !enemies[i]->fired) enemies[i]->aimedShoot(bullets, player->getX(), player->getY());
			break;
		}	
	}
}

//void EnemyLayout::spawnVertical(std::vector<Enemy*>& enemies, int loop, double x, double y, double speed,
//									EnemyType type, std::vector<Bullet*>& bullets, Player*& player) {
//	static Uint32 lastSpawnTime = 0;
//	static int lp = 0;
//	if (currentTime - lastSpawnTime > 200 && lp < loop) {
//		enemies.emplace_back(new Enemy(x, y, speed, type, MovementType::Vertical));
//		lastSpawnTime = currentTime;
//		lp++;
//	}
//
//}