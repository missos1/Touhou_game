#include "headers/Game.hpp"
#include "headers/Player.hpp"
#include "headers/Bullets.hpp"
#include "headers/Enemy.hpp"
#include "headers/Collision.hpp"
#include "headers/SoundManager.hpp"
#include "headers/Items.hpp"
#include "headers/Boss.hpp"

void CollisionCheck::PlayerColli(std::vector<Bullet*>& bullets, Player* player, std::vector<Item*>& items) {
    // Check collision between player and enemy bullets
    
    
    for (int i = static_cast<int>(bullets.size() - 1); i >= 0; i--) { // Iterate through enemy bullets
        SDL_Rect player_grazebox = player->getGrazingBox(); // Get player's grazing box
        SDL_Rect player_hitbox = player->getHitbox(); // Get player's hitbox

        SDL_Rect bullet_hitbox = bullets[i]->getHitbox(); // Get bullet's hitbox

        if (!SDL_HasIntersection(&player_grazebox, &bullet_hitbox) || bullets[i]->getGrazeState()) continue; // Check if bullet grazes player

        bullets[i]->GrazeUpdate(); // Update graze state
        player->updateGraze();
        std::cout << "graze: " << player->getGraze() << std::endl; // Debug: print graze count

        if (!SDL_HasIntersection(&player_hitbox, &bullet_hitbox)) continue; // Check if bullet hits player

        delete bullets[i]; // Delete bullet
        bullets.erase(bullets.begin() + i); // Remove bullet from vector

        if (player->getPlayerstate() == PlayerState::NORMAL) {
            player->updatePlayerhp(-1); // Decrease player's HP
            player->updatePlayerpower(-0.3); // Decrease player's power
            SoundManager::PlaySound("pldead", 0, Game::SE_volume / 2); // Play player hit sound
        }
        //std::cout << "hp: " << player->getPlayerhp() << std::endl; // Debug: print player's HP

        /*if (player->getPlayerhp() <= 0) {
            delete player; // Crash game if player HP is 0 (test)
        }*/
        
    }

    // Check collision between player and items
    ItemGetCalculation(items, player);
}


void CollisionCheck::EnemyColli(std::vector<Bullet*>& bullets, std::vector<Enemy*>& enemies, std::vector<Item*>& items, Player* player) {
    
    for (int i = (int)bullets.size() - 1; i >= 0; i--) {
        for (int j = (int)enemies.size() - 1; j >= 0; j--) { // Iterate through enemies
            SDL_Rect bullet_hitbox = bullets[i]->getHitbox(); // Get bullet's hitbox
            SDL_Rect enemy_hitbox = enemies[j]->getEnHitbox(); // Get enemy's hitbox

            int bullet_dmg = bullets[i]->getDmg(); // Get bullet's damage

            if (!SDL_HasIntersection(&enemy_hitbox, &bullet_hitbox)) continue; // Check if bullet hits enemy

            enemies[j]->updatehp(enemies[j]->getEnemyhp() - bullet_dmg); // Update enemy's HP

            Game::PLAYSCORE += bullet_dmg * 10; // Gain score for every dmg dealt

            //std::cout << "enemies' hp: " << enemies[j]->getEnemyhp() - bullet_dmg << std::endl; // Debug: print enemy's HP
            SoundManager::PlaySound("entakedmg", 0, Game::SE_volume / 4); // Play enemy hit sound

            if (enemies[j]->getEnemyhp() > 0) continue; // Check if enemy is dead

            Game::PLAYSCORE += enemies[j]->getPoint(); // Gain point for every enemies eliminated

			Item::enemy_drop(enemies[j], items, player); // Drop item from enemy

            delete enemies[j];  // Delete enemy
            enemies.erase(enemies.begin() + j); // Remove enemy from vector
            
            delete bullets[i]; // Delete bullet
            bullets.erase(bullets.begin() + i); // Remove bullet from vector
            break;
            
        }
    }
}

void CollisionCheck::ItemGetCalculation(std::vector<Item*>& items, Player* player) {
    for (int i = (int)items.size() - 1; i >= 0; i--) {
        SDL_Rect item_hitbox = items[i]->getHitbox(); // Get item's hitbox
        SDL_Rect player_grazebox = player->getGrazingBox(); // Get player's grazing box
        SDL_Rect player_hitbox = player->getHitbox(); // Get player's hitbox

        if (!SDL_HasIntersection(&player_grazebox, &item_hitbox)) continue; // Check if player collects item

        if (items[i]->getType() == Itemtype::POINT) {
            Game::PLAYSCORE += items[i]->getPoint() + (items[i]->getPoint() * player->getGraze() / 100); // Increase score based on graze count
        }
        else {
            Game::PLAYSCORE += items[i]->getPoint(); // Increase score
        }

        Itemtype type = items[i]->getType(); // Get item type

        switch (type) {
        case Itemtype::FULLPOWER: // Full power item
            player->updatePlayerpower(5.0); // Increase player's power
            break;
        case Itemtype::POWER_S: // Small power item
            player->updatePlayerpower(0.05); // Increase player's power
            break;
        case Itemtype::POWER_L: // Large power item
            player->updatePlayerpower(0.16); // Increase player's power
            break;
        case Itemtype::ONEUP: // 1-Up item
            player->updatePlayerhp(1); // Increase player's HP
            break;
        }

        delete items[i]; // Delete item
        items.erase(items.begin() + i); // Remove item from vector
        SoundManager::PlaySound("collect_item", 0, Game::SE_volume / 2); // Play item collect sound
        
    }
}

void CollisionCheck::DeleleOffScreen(std::vector<Bullet*>& bullets, std::vector<Bullet*>& player_bullets, std::vector<Enemy*>& enemies, std::vector<Item*>& items) {
    for (int i = (int)items.size() - 1; i >= 0; i--) {
        if (items[i]->getY() < -200 ||
            items[i]->getY() > WIN_HEIGHT + 500 ||  // Offscreen deletion
            items[i]->getX() < -200 ||
            items[i]->getX() > WIN_WIDTH + 500) {
            delete items[i]; // Delete item
            items.erase(items.begin() + i); // Remove item from vector
        }
    }

    for (int j = (int)enemies.size() - 1; j >= 0; j--) {
        if (enemies[j]->getY() < -500 ||
            enemies[j]->getY() > 1000 ||  // Offscreen deletion
            enemies[j]->getX() > 1500 ||
            enemies[j]->getX() < -500) {
            delete enemies[j]; // Delete enemy
            enemies.erase(enemies.begin() + j); // Remove enemy from vector
        }
    }

    for (int i = (int)player_bullets.size() - 1; i >= 0; i--) {
        if (player_bullets[i]->getX() > PLAY_AREA_X_MAX + 20 ||
            player_bullets[i]->getX() < PLAY_AREA_X_MIN - 10 ||
            player_bullets[i]->getY() > PLAY_AREA_Y_MAX ||
            player_bullets[i]->getY() < PLAY_AREA_Y_MIN - 10) { // Offscreen deletion
            delete player_bullets[i];
            player_bullets.erase(player_bullets.begin() + i);
        }
    }

    for (int i = static_cast<int>(bullets.size() - 1); i >= 0; i--) {
        if (bullets[i]->getY() < -100 ||
            bullets[i]->getY() > WIN_HEIGHT + 100 ||  // Offscreen deletion
            bullets[i]->getX() < -100 ||
            bullets[i]->getX() > WIN_WIDTH + 100) {
            delete bullets[i]; // Delete bullet
            bullets.erase(bullets.begin() + i); // Remove bullet from vector
        }
    }
}

void CollisionCheck::BossColli(std::vector<Bullet*>& player_bullets, Boss* boss, std::vector<Item*>& items) {
    if (boss->getPhase() == Phase::IDLE) return;

    for (int i = (int)player_bullets.size() - 1; i >= 0; i--) { 
        SDL_Rect boss_hitbox = boss->getBossHitbox();
        SDL_Rect bullet_hitbox = player_bullets[i]->getHitbox(); // Get bullet's hitbox

        if (!SDL_HasIntersection(&boss_hitbox, &bullet_hitbox)) continue; // Check if bullet hits 

        int bullet_dmg = player_bullets[i]->getDmg(); // Get bullet's damage
        boss->takeDamage(bullet_dmg);
        Game::PLAYSCORE += bullet_dmg * 10; // Gain score for every dmg dealt

        SoundManager::PlaySound("entakedmg", 0, Game::SE_volume / 10); // Play enemy hit sound

        delete player_bullets[i]; // Delete bullet
        player_bullets.erase(player_bullets.begin() + i); // Remove bullet from vector
    
    }
}