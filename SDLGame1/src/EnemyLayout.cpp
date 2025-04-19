#include "headers/Enemy.hpp"
#include "headers/Player.hpp"
#include "headers/Game.hpp"
#include "headers/Bullets.hpp"
#include "headers/EnemyLayout.hpp"
#include "headers/SoundManager.hpp"
#include "headers/Boss.hpp"
#include <iostream>
#include <cmath>
#define endl "\n"

void EnemyLayout::stage(std::vector<Enemy*>& enemies, std::vector<Bullet*>& bullets, Player* player, Boss* boss) {
	static double lastexecuteTime = -1.0; // Last execution time for spawning enemies

	double initTime = static_cast<double>(Game::GameStartTime); // Get gameinitTime
	double aggregatedPauseTime = static_cast<double>(Game::GamePauseTotalTime); // Get total pause time after each pause
	double now = static_cast<double>(SDL_GetTicks64()); // Get the current time
	double elapsed = now - initTime - aggregatedPauseTime; // Get the elapsed time
	Game::GamecurrentTime = static_cast<Uint64>(elapsed); // Update the current time
	elapsed = round((elapsed / 1000.0) * 10.0) / 10.0; // rounding to 1 decimal place

	//std::cout << "elapsed: " << elapsed << "sec" << endl;
	//std::cout << "currentTime (Game): " << Game::GamecurrentTime << " ms" << endl;
	//std::cout << "initTime: " << initTime << endl;
	//std::cout << "realSDLtime: " << now << endl;

	if (elapsed < 0.0) return; // If elapsed time is negative, return early

	if (elapsed != lastexecuteTime && Game::state == GameState::PLAYING ) { // Check if the elapsed time is different from the last execution time

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

		if (elapsed == 2.0) {
			SoundManager::PlayMusic("Stage_theme", -1, Game::BGM_volume); // Play stage music
		}

		if (elapsed == 5.0) {
			enemies.emplace_back(new Enemy(1000, -40, -7, EnemyType::SPARKLE, MovementType::DiagonalNESW));
			enemies.emplace_back(new Enemy(1200, -30, -7, EnemyType::SPARKLE, MovementType::DiagonalNESW));
			enemies.emplace_back(new Enemy(920, -20, -7, EnemyType::SPARKLE, MovementType::DiagonalNESW));
			enemies.emplace_back(new Enemy(880, 0, -7, EnemyType::SPARKLE, MovementType::DiagonalNESW));
			enemies.emplace_back(new Enemy(-450, -100, 7, EnemyType::SPARKLE, MovementType::DiagonalNWSE));
			enemies.emplace_back(new Enemy(-50, -70, 7, EnemyType::SPARKLE, MovementType::DiagonalNWSE));
			enemies.emplace_back(new Enemy(-70, -80, 7, EnemyType::SPARKLE, MovementType::DiagonalNWSE));
			enemies.emplace_back(new Enemy(-100, -30, 7, EnemyType::SPARKLE, MovementType::DiagonalNWSE));
			enemies.emplace_back(new Enemy(-200, -10, 7, EnemyType::SPARKLE, MovementType::DiagonalNWSE));
		}



		for (double spawn : spawntime_1) { // Spawn enemies for the second loop
			if (std::abs(elapsed - (10.0 + spawn)) < 0.01) { // Tolerance compare for correct spawning
				//std::cout << "Spawn Time: " << elapsed << endl; //debug
				//std::cout << initTime << endl; // debug

				enemies.emplace_back(new Enemy(-30, -40, 8, EnemyType::SPARKLE, MovementType::DiagonalNWSE));
				enemies.emplace_back(new Enemy(900, -40, -8, EnemyType::SPARKLE, MovementType::DiagonalNESW));

				//std::cout << "Spawning enemies!" << endl; // debug
				//std::cout << "elapsed = " << elapsed << endl; // debug
			}
			if (std::abs(elapsed - (17.0 + spawn)) < 0.01) { // Tolerance compare for correct spawning
				//std::cout << "Spawn Time: " << elapsed << endl; //debug
				//std::cout << initTime << endl; // debug
				enemies.emplace_back(new Enemy(-30, 100, 8, EnemyType::SPARKLE, MovementType::DiagonalNWSE));
				enemies.emplace_back(new Enemy(900, 100, -8, EnemyType::SPARKLE, MovementType::DiagonalNESW));
				enemies.emplace_back(new Enemy(-130, 10, 8, EnemyType::SPARKLE, MovementType::DiagonalNWSE));
				enemies.emplace_back(new Enemy(1000, 10, -8, EnemyType::SPARKLE, MovementType::DiagonalNESW));

				//std::cout << "Spawning enemies!" << endl; // debug
				//std::cout << "elapsed = " << elapsed << endl; // debug
			}
			if (std::abs(elapsed - (27.0 + spawn)) < 0.01) { // Tolerance compare for correct spawning
				//std::cout << "Spawn Time: " << elapsed << endl; //debug
				//std::cout << initTime << endl; // debug

				enemies.emplace_back(new Enemy(200, -30, 0.8, EnemyType::SPARKLE, MovementType::BezierCurve));
				enemies.emplace_back(new Enemy(300, -10, 0.8, EnemyType::SPARKLE, MovementType::BezierCurve));
				enemies.emplace_back(new Enemy(450, -10, 0.8, EnemyType::SPARKLE, MovementType::BezierCurve));
				enemies.emplace_back(new Enemy(550, -30, 0.8, EnemyType::SPARKLE, MovementType::BezierCurve));

				//std::cout << "Spawning enemies!" << endl; // debug
				//std::cout << "elapsed = " << elapsed << endl; // debug
			}
			if (std::abs(elapsed - (66.0 + spawn)) < 0.01) { // Tolerance compare for correct spawning
				//std::cout << "Spawn Time: " << elapsed << endl; // debug
				//std::cout << initTime << endl; // debug

				enemies.emplace_back(new Enemy(430, -110, 8, EnemyType::SPARKLE, MovementType::Vertical));
				enemies.emplace_back(new Enemy(330, -30, 8, EnemyType::SPARKLE, MovementType::Vertical));
				enemies.emplace_back(new Enemy(230, -20, 8, EnemyType::SPARKLE, MovementType::Vertical));
				enemies.emplace_back(new Enemy(130, -10, 8, EnemyType::SPARKLE, MovementType::Vertical));

				enemies.emplace_back(new Enemy(20, 320, 8, EnemyType::SPARKLE, MovementType::Horizontal));
				enemies.emplace_back(new Enemy(-10, 210, 8, EnemyType::SPARKLE, MovementType::Horizontal));
				enemies.emplace_back(new Enemy(1000, 410, -8, EnemyType::SPARKLE, MovementType::Horizontal));
				enemies.emplace_back(new Enemy(1250, 120, -8, EnemyType::SPARKLE, MovementType::Horizontal));

				//std::cout << "Spawning enemies!" << endl; // debug
				//std::cout << "elapsed = " << elapsed << endl; //debug
			}
		}

		for (double spawn : spawntime_0) { // Spawn enemies for the first loop
			if (std::abs(elapsed - (32.0 + spawn)) < 0.01) { // Tolerance compare for correct spawning
				enemies.emplace_back(new Enemy(-40, 300, 3, EnemyType::RED_FA, MovementType::Horizontal));
				//std::cout << "Spawn Time: " << elapsed << endl; // debug
				//std::cout << "Spawning enemies!" << endl; //debug
			}
			if (std::abs(elapsed - (42.0 + spawn)) < 0.01) { // Tolerance compare for correct spawning
				enemies.emplace_back(new Enemy(410, -35, 0.5, EnemyType::RED_FA, MovementType::BezierCurve));
				enemies.emplace_back(new Enemy(550, 0, 0.3, EnemyType::BLUE_FA, MovementType::BezierCurve));
				//std::cout << "Spawn Time: " << elapsed << endl; // debug					  
				//std::cout << "Spawning enemies!" << endl; //debug
			}
			if (std::abs(elapsed - (75.0 + spawn)) < 0.01) { // Tolerance compare for correct spawning
				enemies.emplace_back(new Enemy(-40, 200, 3, EnemyType::RED_FA, MovementType::Horizontal));
				enemies.emplace_back(new Enemy(900, 100, -3, EnemyType::RED_FA, MovementType::Horizontal));
				//std::cout << "Spawn Time: " << elapsed << endl; // debug
				//std::cout << "Spawning enemies!" << endl; //debug
			}
		}

		if (elapsed == 63.0) {
			enemies.emplace_back(new Enemy(600, -80, 2, EnemyType::BLUE_FA, MovementType::Vertical));
			enemies.emplace_back(new Enemy(500, -90, 2, EnemyType::BLUE_FA, MovementType::Vertical));
			enemies.emplace_back(new Enemy(300, -40, 2, EnemyType::BLUE_FA, MovementType::Vertical));
			enemies.emplace_back(new Enemy(400, -70, 2, EnemyType::BLUE_FA, MovementType::Vertical));
			enemies.emplace_back(new Enemy(200, -70, 2, EnemyType::BLUE_FA, MovementType::Vertical));
			enemies.emplace_back(new Enemy(700, -10, 0.4, EnemyType::BLUE_FA, MovementType::BezierCurve));
			enemies.emplace_back(new Enemy(600, -10, 0.4, EnemyType::BLUE_FA, MovementType::BezierCurve));
			enemies.emplace_back(new Enemy(400, -30, 0.4, EnemyType::BLUE_FA, MovementType::BezierCurve));
			enemies.emplace_back(new Enemy(300, -30, 0.4, EnemyType::BLUE_FA, MovementType::BezierCurve));
		}

		if (elapsed == 70.0) { 
			enemies.emplace_back(new Enemy(900, 205, -3, EnemyType::RED_FA, MovementType::Horizontal));
			enemies.emplace_back(new Enemy(1000, 305, -3, EnemyType::RED_FA, MovementType::Horizontal));
			enemies.emplace_back(new Enemy(1200, 245, -3, EnemyType::RED_FA, MovementType::Horizontal));
			enemies.emplace_back(new Enemy(1400, 140, -3, EnemyType::RED_FA, MovementType::Horizontal));
			enemies.emplace_back(new Enemy(880, 150, -3, EnemyType::RED_FA, MovementType::Horizontal));
		}

		if (elapsed == 50.0) { 
			enemies.emplace_back(new Enemy(900, 400, 2, EnemyType::WHITE_FA, MovementType::Lshape));
			enemies.emplace_back(new Enemy(1430, 250, 2, EnemyType::WHITE_FA, MovementType::Lshape));
			enemies.emplace_back(new Enemy(1220, 100, 2, EnemyType::WHITE_FA, MovementType::Lshape));
			enemies.emplace_back(new Enemy(300, -30, 0.4, EnemyType::BLUE_FA, MovementType::BezierCurve));
			enemies.emplace_back(new Enemy(410, 0, 0.4, EnemyType::BLUE_FA, MovementType::BezierCurve));
		}

		if (elapsed == 55.0) {
			enemies.emplace_back(new Enemy(0, 300, 4, EnemyType::WHITE_FA, MovementType::Lshape));
			enemies.emplace_back(new Enemy(-20, 250, 4, EnemyType::WHITE_FA, MovementType::Lshape));
			/*std::cout << "Spawn Time: " << elapsed << endl;
			std::cout << "Spawning enemies!" << endl;*/
		}
		if (elapsed == 56.0) {
			enemies.emplace_back(new Enemy(-20, 150, 4, EnemyType::WHITE_FA, MovementType::Lshape));
		}

		if (elapsed == 58.0) {
			enemies.emplace_back(new Enemy(-70, 200, 4, EnemyType::WHITE_FA, MovementType::Lshape));
		}

		if (elapsed == 60.0) {
			enemies.emplace_back(new Enemy(-20, 100, 4, EnemyType::WHITE_FA, MovementType::Lshape));
		}

		static std::unordered_map<Enemy*, Uint64> enemyLastShootTime; // Store the last shoot time for each enemy

		for (int i = (int)enemies.size() - 1; i >= 0; i--) {
			if (enemies[i]->getX() < PLAY_AREA_X_MAX &&
				enemies[i]->getX() > PLAY_AREA_X_MIN &&
				enemies[i]->getY() < PLAY_AREA_Y_MAX &&
				enemies[i]->getY() > PLAY_AREA_Y_MIN) {

				switch (enemies[i]->getType()) {
				case EnemyType::RED_FA:
					if (Game::GamecurrentTime - enemyLastShootTime[enemies[i]] > 1000) {
						enemyLastShootTime[enemies[i]] = Game::GamecurrentTime;
						enemies[i]->rndriceShoot(bullets, 4);
					}
					break;

				case EnemyType::WHITE_FA:
					if (Game::GamecurrentTime - enemyLastShootTime[enemies[i]] > 1000) {
						enemyLastShootTime[enemies[i]] = Game::GamecurrentTime;
						enemies[i]->rndriceShoot(bullets, 3);
					}

					if (enemies[i]->getY() <= (enemies[i]->initY - 10) && !enemies[i]->fired) {
						enemies[i]->circleroundShoot(bullets, 40);
					}
					break;

				case EnemyType::BLUE_FA:
					if (enemies[i]->getY() >= 250 && !enemies[i]->fired) {
						enemies[i]->aimedShoot(bullets, player->getX(), player->getY());
					}
					break;
				case EnemyType::SPARKLE:
					if (Game::GamecurrentTime - enemyLastShootTime[enemies[i]] > 1000) {
						enemyLastShootTime[enemies[i]] = Game::GamecurrentTime;
						enemies[i]->rndcircleShoot(bullets, 4);
					}
					break;
				}
			}
			else {
				enemyLastShootTime[enemies[i]] = Game::GamecurrentTime + rand() % 2000; // Reset the last shoot time if the enemy is off-screen
			}
		}

		if (elapsed >= 90.0) {
			if (elapsed == 90.0) {
				SoundManager::StopMusic();
			}
			if (elapsed == 91.0) {
				SoundManager::PlayMusic("Boss_theme", -1, Game::BGM_volume);
			}
			
			switch (boss->getPhase()) {
			case Phase::IDLE:
				boss->moveinscreen();
				break;
			case Phase::PHASE0:
				boss->phase0(bullets, player);
				break;
			case Phase::PHASE0_SC:
				boss->phase0_spellcard(bullets, player);
				break;
			case Phase::PHASE1:
				boss->phase1(bullets, player);
				break;
			case Phase::PHASE1_SC:
				boss->phase1_spellcard(bullets, player);
				break;
			}
			//std::cout << boss->getBosshp() << endl;
		}
	}
}


