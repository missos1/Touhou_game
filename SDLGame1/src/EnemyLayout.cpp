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
	static double lastexecuteTime = -1.0; // Last execution time for spawning enemies

	double initTime = Game::GameStartTime / 1000; // Initial game start time in seconds
	double elapsed = SDL_GetTicks() / 1000.0 - initTime; // Time elapsed since the game started in seconds
	elapsed = round(elapsed * 10) / 10 + 0.0; // Round elapsed time to 1 decimal place

	//std::cout << "elapsed: " << elapsed << endl;
	//std::cout << "initTime: " << initTime << endl;
	//std::cout << "realtime: " << tmp << endl;

	if (elapsed < 0.0) return; // If elapsed time is negative, return early

	if (elapsed != lastexecuteTime) { // Check if the elapsed time is different from the last execution time
		lastexecuteTime = elapsed; // Update the last execution time

		int loop_0 = 10; // Number of enemies in the first loop
		int loop_1 = 20; // Number of enemies in the second loop

		double interval_0 = 0.5; // Interval for the first loop
		double interval_1 = 0.3; // Interval for the second loop

		std::vector<double> spawntime_0; // Vector to store spawn times for the first loop
		std::vector<double> spawntime_1; // Vector to store spawn times for the second loop

		for (int i = 1; i <= loop_0; i++) { // Populate spawn times for the first loop
			double x = interval_0 * (double)i;
			spawntime_0.push_back(x);
		}
		for (int i = 1; i <= loop_1; i++) { // Populate spawn times for the second loop
			double y = interval_1 * (double)i;
			spawntime_1.push_back(y);
		}
		for (double spawn : spawntime_0) { // Spawn enemies for the first loop
			if (std::abs(elapsed - (15.0 + spawn)) < 0.01) { // Tolerance compare for correct spawning
				enemies.emplace_back(new Enemy(800, 200, -4, EnemyType::RED_FA, MovementType::Horizontal));
				//std::cout << "Spawn Time: " << elapsed << endl; // debug
				//std::cout << "Spawning enemies!" << endl; //debug
			}
		}

		for (double spawn : spawntime_1) { // Spawn enemies for the second loop
			if (std::abs(elapsed - (10.0 + spawn)) < 0.01) { // Tolerance compare for correct spawning
				//std::cout << "Spawn Time: " << elapsed << endl; // debug
				//std::cout << initTime << endl; // debug

				enemies.emplace_back(new Enemy(-30, 150, 11, EnemyType::SPARKLE, MovementType::Horizontal));

				//std::cout << "Spawning enemies!" << endl; // debug
				//std::cout << "elapsed = " << elapsed << endl; //debug
			}
			if (std::abs(elapsed - (11.0 + spawn)) < 0.01) { // Tolerance compare for correct spawning
				//std::cout << "Spawn Time: " << elapsed << endl; //debug
				//std::cout << initTime << endl; // debug

				enemies.emplace_back(new Enemy(-30, -40, 8, EnemyType::SPARKLE, MovementType::DiagonalNWSE));

				//std::cout << "Spawning enemies!" << endl; // debug
				//std::cout << "elapsed = " << elapsed << endl; // debug
			}
		}
		//if (elapsed == 5.0) { // Spawn specific enemies at 5 seconds
		//	enemies.emplace_back(new Enemy(-30, 200, 2, EnemyType::WHITE_FA, MovementType::Lshape));
		//	//enemies.emplace_back(new Enemy(-30, 200, 2, EnemyType::WHITE_FA, MovementType::Lshape));
		//	enemies.emplace_back(new Enemy(740, 300, 2, EnemyType::WHITE_FA, MovementType::Lshape));
		//	enemies.emplace_back(new Enemy(830, 250, 2, EnemyType::WHITE_FA, MovementType::Lshape));
		//	enemies.emplace_back(new Enemy(920, 200, 2, EnemyType::WHITE_FA, MovementType::Lshape));
		//	//std::cout << "Spawn Time: " << elapsed << endl;
		//	//std::cout << "Spawning enemies!" << endl;
		//}
	}

	Uint32 currentTime = SDL_GetTicks(); // Get the current time in milliseconds

	static std::unordered_map<Enemy*, Uint32> enemyLastShootTime; // Store the last shoot time for each enemy

	for (int i = (int)enemies.size() - 1; i >= 0; i--) { // Iterate through the enemies
		/*if (enemies[i]->getX() >= 350 && enemies[i]->getX() <= 550)*/
		switch (enemies[i]->getType()) { // Check the type of the enemy
		case EnemyType::RED_FA:
			if (currentTime - enemyLastShootTime[enemies[i]] > 1000) { // Check if enough time has passed since the last shot
				enemyLastShootTime[enemies[i]] = currentTime; // Update the last shoot time
				enemies[i]->rndriceShoot(bullets, 5); // Enemy shoots bullets randomly
			}
			break;
		case EnemyType::WHITE_FA:
			//if (enemies[i]->getY() >= 100 && !enemies[i]->fired) enemies[i]->aimedShoot(bullets, player->getX(), player->getY());
			if (enemies[i]->getY() <= (enemies[i]->initY - 10) && !enemies[i]->fired) { // Check if the enemy is in position to shoot
				enemies[i]->circleroundShoot(bullets, 60); // Enemy shoots bullets in a circular pattern
			}
			break;
		}
	}
}

