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

using json = nlohmann::json;

EnemyType EnemyLayout::getEnemyTypeEnum(const std::string& typeStr) {
	if (typeStr == "SPARKLE") return EnemyType::SPARKLE;
	if (typeStr == "WHITE_FA") return EnemyType::WHITE_FA;
	if (typeStr == "BLUE_FA") return EnemyType::BLUE_FA;
	if (typeStr == "RED_FA") return EnemyType::RED_FA;

	return EnemyType::SPARKLE; // Default fallback
}

MovementType EnemyLayout::getMovementTypeEnum(const std::string& moveStr) {
	if (moveStr == "DiagonalNESW") return MovementType::DiagonalNESW;
	if (moveStr == "DiagonalNWSE") return MovementType::DiagonalNWSE;
	if (moveStr == "Lshape") return MovementType::Lshape;
	if (moveStr == "BezierCurve") return MovementType::BezierCurve;
	if (moveStr == "Horizontal") return MovementType::Horizontal;
	if (moveStr == "Vertical") return MovementType::Vertical;

	return MovementType::Horizontal; // Default fallback
}

void EnemyLayout::loadEnemyDatafromjson(const std::string& filename) {
	std::ifstream file(filename);
	if (!file.is_open()) {
		std::cerr << "Error opening file: " << filename << endl;
		return;
	}

	json data; file >> data;

	for (const auto& entry : data.items()) {
		double spawnTime = std::stod(entry.key()); // Get the spawnTime key (e.g., "5.0")

		for (const auto& enemyEntry : entry.value()) {
			EnemySpawnData enemyData;
			enemyData.spawnTime = spawnTime;
			enemyData.x = enemyEntry["x"];
			enemyData.y = enemyEntry["y"];
			enemyData.speed = enemyEntry["speed"];
			enemyData.type = getEnemyTypeEnum((enemyEntry["type"]));
			enemyData.movementType = static_cast<MovementType>(getMovementTypeEnum(enemyEntry["movementType"]));
			enemySpawnDataList.push_back(enemyData);
		}
	}
	file.close(); // Close the file after reading
}

std::vector<EnemyLayout::EnemySpawnData> EnemyLayout::enemySpawnDataList; // Static variable to store enemy spawn data
bool EnemyLayout::loaded_json = false;

void EnemyLayout::EnemyShootCondition(std::vector<Enemy*>& enemies, std::vector<Bullet*>& bullets, Player* player) {
	static std::unordered_map<Enemy*, Uint64> enemyLastShootTime; // Store the last shoot time for each enemy

	int player_hitbox_x = player->getX(); // Get the player's hitbox
	int player_hitbox_y = player->getY(); // Get the player's hitbox

	for (int i = (int)enemies.size() - 1; i >= 0; i--) {
		if (enemies[i]->getX() >= PLAY_AREA_X_MAX ||
			enemies[i]->getX() <= PLAY_AREA_X_MIN ||
			enemies[i]->getY() >= PLAY_AREA_Y_MAX ||
			enemies[i]->getY() <= PLAY_AREA_Y_MIN) {
			enemyLastShootTime[enemies[i]] = Game::GamecurrentTime + rand() % 2000; // Reset the last shoot time if the enemy is off-screen
			continue; // Skip the rest of the loop for this enemy
		}

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
				enemies[i]->aimedShoot(bullets, player_hitbox_x, player_hitbox_y);
			}
			break;
		case EnemyType::SPARKLE:
			if (Game::GamecurrentTime - enemyLastShootTime[enemies[i]] > 1000) {
				enemyLastShootTime[enemies[i]] = Game::GamecurrentTime;
				enemies[i]->rndcircleShoot(bullets, 3);
			}
			break;
		}
	}
}

void EnemyLayout::stage(std::vector<Enemy*>& enemies, std::vector<Bullet*>& bullets, Player* player, Boss* boss) {
	if (loaded_json == false) {
		loadEnemyDatafromjson("res/Enemy/stage_0_enemies.json"); // Load enemy data from JSON file
		loaded_json = true; // Set loaded_json to true to prevent reloading
	}

	static double lastexecuteTime = -1.0; // Last execution time for spawning enemies

	double initTime = static_cast<double>(Game::GameStartTime);                 // Get gameinitTime
	double aggregatedPauseTime = static_cast<double>(Game::GamePauseTotalTime); // Get total pause time after each pause
	double now = static_cast<double>(SDL_GetTicks64());							// Get the current time
	double elapsed = now - initTime - aggregatedPauseTime;					    // Get the elapsed time
	Game::GamecurrentTime = static_cast<Uint64>(elapsed);						// Update the current time
	elapsed = round((elapsed / 1000.0) * 10.0) / 10.0 + 89.0;						    // rounding to 1 decimal place

	//std::cout << "elapsed: " << elapsed << "sec" << endl;
	//std::cout << "currentTime (Game): " << Game::GamecurrentTime << " ms" << endl;
	//std::cout << "initTime: " << initTime << endl;
	//std::cout << "realSDLtime: " << now << endl;

	if (elapsed < 0.0) return; // If elapsed time is negative, return early


	if (elapsed == lastexecuteTime || Game::state != GameState::PLAYING) return; // Check if the elapsed time is different from the last execution time

	lastexecuteTime = elapsed; // Update the last execution time

	if (elapsed == 2.0) {
		SoundManager::PlayMusic("Stage_theme", -1, Game::BGM_volume); // Play stage music
	}

	for (const auto& spawnData : enemySpawnDataList) {
		if (isElapsed(elapsed, spawnData.spawnTime)) { // Check if the elapsed time matches the spawn time
			// Spawn the enemy with the appropriate data
			enemies.emplace_back(new Enemy(spawnData.x, spawnData.y, spawnData.speed, spawnData.type, spawnData.movementType));
		}
	}

	EnemyShootCondition(enemies, bullets, player); // Call the enemy shooting condition function

	if (elapsed >= 90.0) {
		boss->getStage_elapsed(elapsed); // Update the boss with the elapsed time

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
		case Phase::PHASE2:
			boss->phase2(bullets, player);
			break;
		case Phase::PHASE2_SC:
			boss->phase2_spellcard(bullets, player);
			break;
		}
		//std::cout << boss->getBosshp() << endl;
	}
}


