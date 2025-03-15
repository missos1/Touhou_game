#include "headers/Enemy.hpp"
#include "headers/Player.hpp"
#include "headers/Game.hpp"
#include "headers/Bullets.hpp"
#include "headers/EnemyLayout.hpp"

void EnemyLayout::spawnHorizontalWave(std::vector<Enemy*>& enemies, int loop, std::vector<Bullet*>& bullets, Player* &player) {
	static Uint32 lastSpawnTime = 0;
	Uint32 currentTime = SDL_GetTicks();
	static int lp = 0;
	if (currentTime - lastSpawnTime > 500 && lp < loop) {
		enemies.emplace_back(new Enemy(32, 50, 7, EnemyType::WHITE_FA, MovementType::Horizontal));
		enemies.emplace_back(new Enemy(1000, 50, -7, EnemyType::WHITE_FA, MovementType::Horizontal));
		lastSpawnTime = currentTime;
		lp++;
	}
	for (int i = (int)enemies.size() - 1; i >= 0; i--) {
		//if (enemies[i]->getX() >= 350 && !enemies[i]-> fired) enemies[i]->enemyShoot(bullets);
		int playerX = player->getX(); 
		int playerY = player->getY();
		if (enemies[i]->getX() >= 350 && !enemies[i]->fired) enemies[i]->testShoot(bullets, playerX ,playerY);
	}
}

void EnemyLayout::spawnVerticalWave(std::vector<Enemy*>& enemies, int loop) {
	static Uint32 lastSpawnTime = 0;
	Uint32 currentTime = SDL_GetTicks();
	static int lp = 0;
	if (currentTime - lastSpawnTime > 200 && lp < loop) {
		enemies.emplace_back(new Enemy(64, -50, 7, EnemyType::WHITE_FA, MovementType::Vertical));
		enemies.emplace_back(new Enemy(300, -50, -7, EnemyType::WHITE_FA, MovementType::Vertical));
		lastSpawnTime = currentTime;
		lp++;
	}
}

void EnemyLayout::wave1(std::vector<Enemy*>& enemies, int loop, std::vector<Bullet*>& bullets, Player*& player) {

}