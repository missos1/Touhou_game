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
    static void stage(std::vector<Enemy*>& enemies, std::vector<Bullet*>& bullets, Player* player);
private:

};

#endif 