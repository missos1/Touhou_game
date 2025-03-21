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
class StageTimeline;
class StageEvent;
class Timer;

class EnemyLayout {
public:
    static void wave1(std::vector<Enemy*>& enemies, std::vector<Bullet*>& bullets, Player* player);
    static void wave2(std::vector<Enemy*>& enemies, std::vector<Bullet*>& bullets, Player* player);
    static void spawnBezierWave(std::vector<Enemy*>& enemies, int loop);

    static void spawnHorizontal(std::vector<Enemy*>& enemies, double x, double y, double speed,
    EnemyType type, std::vector<Bullet*>& bullets, Player* &player);

    static void spawnGroupOf3(std::vector<Enemy*>& enemies, int loop);
    static void spawnVertical(std::vector<Enemy*>& enemies, double x, double y, double speed, 
    EnemyType type, std::vector<Bullet*>& bullets, Player* player);
};

#endif 