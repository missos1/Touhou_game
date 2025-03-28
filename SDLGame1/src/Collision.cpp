#include "headers/Game.hpp"
#include "headers/Player.hpp"
#include "headers/Bullets.hpp"
#include "headers/Enemy.hpp"
#include "headers/Collision.hpp"
#include "headers/SoundManager.hpp"
#include "headers/Items.hpp"

void CollisionCheck::PlayerColli(std::vector<Bullet*>& bullets, Player* player, std::vector<Item*>& items) {
    // Check collision between player and enemy bullets
    
    
    for (int i = static_cast<int>(bullets.size() - 1); i >= 0; i--) { // Iterate through enemy bullets
        SDL_Rect player_grazebox = player->getGrazingBox(); // Get player's grazing box
        SDL_Rect player_hitbox = player->getHitbox(); // Get player's hitbox

        SDL_Rect bullet_hitbox = bullets[i]->getHitbox(); // Get bullet's hitbox

        if (SDL_HasIntersection(&player_grazebox, &bullet_hitbox) && !bullets[i]->getGrazeState()) { // Check if bullet grazes player
            bullets[i]->GrazeUpdate(); // Update graze state
            player->updateGraze();
            std::cout << "graze: " << player->getGraze() << std::endl; // Debug: print graze count
        }

        if (SDL_HasIntersection(&player_hitbox, &bullet_hitbox)) { // Check if bullet hits player
            delete bullets[i]; // Delete bullet
            bullets.erase(bullets.begin() + i); // Remove bullet from vector
            player->updatePlayerhp(-1); // Decrease player's HP
            player->updatePlayerpower(-0.3); // Decrease player's power
            SoundManager::PlaySound("pldead", 0, 64); // Play player hit sound
            //std::cout << "hp: " << player->getPlayerhp() << std::endl; // Debug: print player's HP

            /*if (player->getPlayerhp() <= 0) {
                delete player; // Crash game if player HP is 0 (test)
            }*/
        }

        else if (bullets[i]->getY() < -100 ||
            bullets[i]->getY() > WIN_HEIGHT + 100 ||  // Offscreen deletion
            bullets[i]->getX() < -100 ||
            bullets[i]->getX() > WIN_WIDTH + 100) {
            delete bullets[i]; // Delete bullet
            bullets.erase(bullets.begin() + i); // Remove bullet from vector
        }
    }

    // Check collision between player and items
    for (int i = (int)items.size() - 1; i >= 0; i--) {
        SDL_Rect item_hitbox = items[i]->getHitbox(); // Get item's hitbox
        SDL_Rect player_grazebox = player->getGrazingBox(); // Get player's grazing box
        SDL_Rect player_hitbox = player->getHitbox(); // Get player's hitbox

        if (SDL_HasIntersection(&player_grazebox, &item_hitbox)) { // Check if player collects item
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
                player->updatePlayerpower(0.02); // Increase player's power
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
            SoundManager::PlaySound("collect_item", 0, 64); // Play item collect sound
        }

        else if (items[i]->getY() < -200 ||
                items[i]->getY() > WIN_HEIGHT + 500 ||  // Offscreen deletion
                items[i]->getX() < -200 ||
                items[i]->getX() > WIN_WIDTH + 500) {
                delete items[i]; // Delete item
                items.erase(items.begin() + i); // Remove item from vector
        }
    }
}

void CollisionCheck::EnemyColli(std::vector<Bullet*>& bullets, std::vector<Enemy*>& enemies, std::vector<Item*>& items, Player* player) {
    static int countspawn = 1; // Count for item spawn
    for (int i = (int)bullets.size() - 1; i >= 0; i--) {
        for (int j = (int)enemies.size() - 1; j >= 0; j--) { // Iterate through enemies
            SDL_Rect bullet_hitbox = bullets[i]->getHitbox(); // Get bullet's hitbox
            SDL_Rect enemy_hitbox = enemies[j]->getEnHitbox(); // Get enemy's hitbox

            int bullet_dmg = bullets[i]->getDmg(); // Get bullet's damage

            if (SDL_HasIntersection(&enemy_hitbox, &bullet_hitbox)) { // Check if bullet hits enemy
                enemies[j]->updatehp(enemies[j]->getEnemyhp() - bullet_dmg); // Update enemy's HP

                Game::PLAYSCORE += bullet_dmg * 10; // Gain score for every dmg dealt

                std::cout << "enemies' hp: " << enemies[j]->getEnemyhp() - bullet_dmg << std::endl; // Debug: print enemy's HP
                SoundManager::PlaySound("entakedmg", 0, 16); // Play enemy hit sound

                if (enemies[j]->getEnemyhp() <= 0) { // Check if enemy is dead

                    Game::PLAYSCORE += enemies[j]->getPoint(); // Gain point for every enemies eliminated

                    Itemtype type;

                    if (enemies[j]->getType() == EnemyType::RED_FA ||
                        enemies[j]->getType() == EnemyType::WHITE_FA ||
                        enemies[j]->getType() == EnemyType::BLUE_FA ) {
                        for (int k = 0; k < 3; ++k) {
                            int randomX = enemy_hitbox.x + (rand() % 201 - 100);  // Random X offset between -100 and 100
                            int randomY = enemy_hitbox.y + (rand() % 201 - 100);  // Random Y offset between -100 and 100
                            items.emplace_back(new Item(randomX, randomY, Itemtype::POINT));
                        }

                        items.emplace_back(new Item(enemy_hitbox.x, enemy_hitbox.y, Itemtype::POWER_L));

                        if (enemies[j]->getType() == EnemyType::WHITE_FA) {
                            items.emplace_back(new Item(enemy_hitbox.x + 60, enemy_hitbox.y - 40, Itemtype::POWER_L));
                        }
                    }         

                    if (player->getPlayerpowerlv() < 5.0) { // Determine item type
                        if (countspawn % 31 == 0) {
                            type = Itemtype::POWER_L;
                            countspawn = 1;
                        }
                        else if (countspawn % 2 == 0){
                            type = Itemtype::POINT;
                        }
                        else {
                            type = Itemtype::POWER_S;
                        }
                    }
                    else {
                        type = Itemtype::POINT;
                    }

                    countspawn++;

                    items.emplace_back(new Item(enemy_hitbox.x, enemy_hitbox.y, type)); // Spawn item
                    SoundManager::PlaySound("endie0", 0, 16); // Play enemy death sound

                    delete enemies[j];  // Delete enemy
                    enemies.erase(enemies.begin() + j); // Remove enemy from vector
                }
                delete bullets[i]; // Delete bullet
                bullets.erase(bullets.begin() + i); // Remove bullet from vector
                break;
            }
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

    for (int i = (int)bullets.size() - 1; i >= 0; i--) {
        if (bullets[i]->getX() > PLAY_AREA_X_MAX + 20 ||
            bullets[i]->getX() < PLAY_AREA_X_MIN - 10 ||
            bullets[i]->getY() > PLAY_AREA_Y_MAX ||
            bullets[i]->getY() < PLAY_AREA_Y_MIN - 10) { // Offscreen deletion
            delete bullets[i];
            bullets.erase(bullets.begin() + i);
        }
    }
}

