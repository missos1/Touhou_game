#ifndef ENEMYLAYOUT_HPP
#define	ENEMYLAYOUT_HPP

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <vector>
#include <sstream>
#include <fstream>
#include <string>
#include "json.hpp"

class Enemy;
class Game;
class Bullet;
class Player;
class Boss;

class EnemyLayout {
public:
    static void stage(std::vector<Enemy*>& enemies, std::vector<Bullet*>& bullets, Player* player, Boss* boss);
	static bool loaded_json; // Flag to check if JSON data is loaded
private:
	struct EnemySpawnData {
		double spawnTime;
		int x;
		int y;
		double speed;
		EnemyType type;
		MovementType movementType;
	};

	static EnemyType getEnemyTypeEnum(const std::string& typeStr);
	static MovementType getMovementTypeEnum(const std::string& moveStr);

	static inline bool isElapsed(double elapsed, double spawnTime) {
		return std::abs(elapsed - spawnTime) < 0.01; // Tolerance compare for correct spawning
	}

	static std::vector<EnemySpawnData> enemySpawnDataList;
	static void loadEnemyDatafromjson(const std::string& filename);
	static void EnemyShootCondition(std::vector<Enemy*>& enemies, std::vector<Bullet*>& bullets, Player* player);
	
};


#endif 