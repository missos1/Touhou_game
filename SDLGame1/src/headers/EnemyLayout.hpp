#ifndef ENEMYLAYOUT_HPP
#define	ENEMYLAYOUT_HPP

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <vector>

class Enemy;
class Game;
class Bullet;
class Player;

class EnemyLayout {
public:
    static void spawnBezierWave(std::vector<Enemy*>& enemies, int loop);
    static void spawnHorizontalWave(std::vector<Enemy*>& enemies, int loop, std::vector<Bullet*>& bullets, Player* &player);
    static void spawnGroupOf3(std::vector<Enemy*>& enemies, int loop);
    static void spawnVerticalWave(std::vector<Enemy*>& enemies, int loop);
};

#endif