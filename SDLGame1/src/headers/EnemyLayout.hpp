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
    static void spawnBezierWave(std::vector<Enemy*>& enemies, double time);
    static void spawnHorizontalWave(std::vector<Enemy*>& enemies);
    static void spawnGroupOf3(std::vector<Enemy*>& enemies, double time);
    static void spawnStationaryEnemies(std::vector<Enemy*>& enemies, double time);
};

#endif