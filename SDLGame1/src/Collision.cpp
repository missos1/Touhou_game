#include "headers/Game.hpp"
#include "headers/Player.hpp"
#include "headers/Bullets.hpp"
#include "headers/Enemy.hpp"
#include "headers/Collision.hpp"
#include "headers/SoundManager.hpp"

void CollisionCheck::PlayerColli(std::vector<Bullet*>& bullets, Player*& player) {
    for (int i = (int)bullets.size() - 1; i >= 0; i--) { // update enemies' bullets loop
        static int grazecount = 0; // graze count
    
        SDL_Rect bullet_hitbox = bullets[i]->getHitbox();
        SDL_Rect player_grazebox = player->getGrazingBox();
        SDL_Rect player_hitbox = player->getHitbox();

        if (SDL_HasIntersection(&player_grazebox, &bullet_hitbox) && !bullets[i]->getGrazeState()) { // grazing bullets for more points (future ver)
            bullets[i]->GrazeUpdate(); // update if grazed
            SoundManager::PlaySound("graze", 0, 64); // play graze sound
            //std::cout << "graze: " << ++grazecount << std::endl; // debug
        }

        if (SDL_HasIntersection(&player_hitbox, &bullet_hitbox)) { // hp collision
            delete bullets[i]; // delete bullet when it collided with player
            bullets.erase(bullets.begin() + i); // delete its postion
            player->updatePlayerhp();
            SoundManager::PlaySound("pldead", 0, 64); // play sound when player dies
            //std::cout << "hp: " << player->getPlayerhp() << std::endl; //debug

            /*if (player->getPlayerhp() <= 0) {
                delete player; // crash game lmao
            }*/
        }

        if (bullets[i]->getY() < -100 ||
            bullets[i]->getY() > WIN_HEIGHT + 100 ||  // offscreen deletion
            bullets[i]->getX() < -100 ||
            bullets[i]->getX() > WIN_WIDTH + 100) {
            delete bullets[i];
            bullets.erase(bullets.begin() + i); // erase bullets
        }
    }
}

void CollisionCheck::EnemyColli(std::vector<Bullet*>& bullets, std::vector<Enemy*>& enemies) {
    for (int i = (int)bullets.size() - 1; i >= 0; i--) {
        for (int j = (int)enemies.size() - 1; j >= 0; j--) { // update enemies
            SDL_Rect bullet_hitbox = bullets[i]->getHitbox();
            SDL_Rect enemy_hitbox = enemies[j]->getEnHitbox();

            int bullet_dmg = bullets[i]->getDmg();
            int enemy_hp = enemies[j]->getEnemyhp();

            if (SDL_HasIntersection(&enemy_hitbox, &bullet_hitbox)) { // enemy collision
                enemies[j]->updatehp(enemy_hp - bullet_dmg);
                std::cout << "hp: " << enemy_hp - bullet_dmg << std::endl;
                SoundManager::PlaySound("entakedmg", 0, 32);
                if (enemies[j]->getEnemyhp() <= 0) {
                    delete enemies[j];
                    enemies.erase(enemies.begin() + j);
                    SoundManager::PlaySound("endie0", 0, 64);
                }
                delete bullets[i];
                bullets.erase(bullets.begin() + i); 
                break;
            }
        }
    }

    for (int j = (int)enemies.size() - 1; j >= 0; j--) {
        if (enemies[j]->getY() < -100 ||
            enemies[j]->getY() > 1000 ||
            enemies[j]->getX() > 1500 ||
            enemies[j]->getX() < -100) {
            delete enemies[j];
            enemies.erase(enemies.begin() + j); // erase enemies
        }
    }

    for (int i = (int)bullets.size() - 1; i >= 0; i--) {
        if (bullets[i]->getY() < 0) {
            delete bullets[i];
            bullets.erase(bullets.begin() + i);
        }
    }
}