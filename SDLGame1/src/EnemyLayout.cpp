#include "headers/Enemy.hpp"
#include "headers/Player.hpp"
#include "headers/Game.hpp"
#include "headers/Bullets.hpp"
#include "headers/EnemyLayout.hpp"

void EnemyLayout::spawnHorizontalWave(std::vector<Enemy*>& enemies) {
	static Uint32 lastSpawnTime = 0;
	Uint32 currentTime = SDL_GetTicks();
	static int loop = 0;
	if (currentTime - lastSpawnTime > 200 && loop < 5) {
		enemies.emplace_back(new Enemy(32, 50, 4, EnemyType::RED_FA, MovementType::Horizontal));
		lastSpawnTime = currentTime;
		loop++;
	}
}