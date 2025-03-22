#include "headers/Enemy.hpp"
#include "headers/Player.hpp"
#include "headers/Game.hpp"
#include "headers/Bullets.hpp"
#include "headers/EnemyLayout.hpp"
#include "headers/SoundManager.hpp"
#include <iostream>
#include <cmath>
#define endl "\n"

void EnemyLayout::wave1(std::vector<Enemy*>& enemies, std::vector<Bullet*>& bullets, Player* player) {
	static double lastexecuteTime = -1.0;

	double initTime = Game::GameStartTime / 1000; // check the time to spawn enemies
	double elapsed = SDL_GetTicks() / 1000.0 - initTime; // get the time elapsed since the game started
	elapsed = round(elapsed * 10) / 10 + 0.0; // round to 1 decimal place


	//std::cout << "elapsed: " << elapsed << endl;
	//std::cout << "initTime: " << initTime << endl;

	//std::cout << "realtime: " << tmp << endl;

	if (elapsed < 0.0) return;


	if (elapsed != lastexecuteTime) {

		lastexecuteTime = elapsed;
		//std::cout << "lastexe = " << lastexecuteTime << endl;
		
		int loop = 10; 

		double interval = 0.5;

		std::vector<double> spawntime;

		for (int i = 1; i <= loop; i++) {
			double x = interval * (double) i;
			spawntime.push_back(x);
		}

		for (double spawn : spawntime) {
			if (std::abs(elapsed - (10.0 + spawn)) < 0.01) { // torelance compare for correct spawning
				std::cout << "Spawn Time: " << elapsed << endl;
				//std::cout << initTime << endl;
				enemies.emplace_back(new Enemy(-30, 400, 2, EnemyType::RED_FA, MovementType::Horizontal));
				//std::cout << "Spawning enemies!" << endl;

				//std::cout << "elapsed = " << elapsed << endl;
			}
			if (std::abs(elapsed - (15.0 + spawn)) < 0.01) { // torelance compare for correct spawning
				enemies.emplace_back(new Enemy(800, 500, -2, EnemyType::RED_FA, MovementType::Horizontal));
				std::cout << "Spawn Time: " << elapsed << endl;
				std::cout << "Spawning enemies!" << endl;
			}
		}
		if (elapsed == 20.0) {
				enemies.emplace_back(new Enemy(600, -30, 2, EnemyType::WHITE_FA, MovementType::Vertical));
				enemies.emplace_back(new Enemy(300, -30, 2, EnemyType::WHITE_FA, MovementType::Vertical));
				std::cout << "Spawn Time: " << elapsed << endl;
				std::cout << "Spawning enemies!" << endl;
		}
	}

	Uint32 currentTime = SDL_GetTicks();

	static std::unordered_map<Enemy*, Uint32> enemyLastShootTime; // storing timimg for each enemies

	for (int i = (int)enemies.size() - 1; i >= 0; i--) {
		/*if (enemies[i]->getX() >= 350 && enemies[i]->getX() <= 550)*/
		switch (enemies[i]->getType()) {
		case EnemyType::RED_FA:
			if (currentTime - enemyLastShootTime[enemies[i]] > 1000) {
				enemyLastShootTime[enemies[i]] = currentTime;
				enemies[i]->rndriceShoot(bullets, 5);
				
			}
			break;
		case EnemyType::WHITE_FA:
			//if (enemies[i]->getY() >= 100 && !enemies[i]->fired) enemies[i]->aimedShoot(bullets, player->getX(), player->getY());
			if (enemies[i]->getY() >= 100 && !enemies[i]->fired) {
				enemies[i]->circleroundShoot(bullets, 60);
			}
			break;
		}	
	}
}

//void EnemyLayout::spawnVertical(std::vector<Enemy*>& enemies, int loop, double x, double y, double speed,
//									EnemyType type, std::vector<Bullet*>& bullets, Player* player) {
//	static Uint32 lastSpawnTime = 0;
//	static int lp = 0;
//	if (currentTime - lastSpawnTime > 200 && lp < loop) {
//		enemies.emplace_back(new Enemy(x, y, speed, type, MovementType::Vertical));
//		lastSpawnTime = currentTime;
//		lp++;
//	}
//
//}