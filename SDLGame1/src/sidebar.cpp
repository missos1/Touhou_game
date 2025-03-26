#include "headers/sidebar.hpp"
#include "headers/Game.hpp"
#include "headers/Player.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

Sidebar::Sidebar()
    : tileSize(0), bg_texture(nullptr), title_texture(nullptr), graze_texture(nullptr), 
    power_texture(nullptr), destRect{ 0, 0, 0, 0 }, desRect_title{ 0, 0, 0, 0 }, destRect_digit{ 0,0,0,0 },
    srcRect_digit{ 0,0,0,0 }, destRect_power{ 0, 0, 0, 0 }, destRect_hp{ 0, 0, 0, 0} {

    bg_texture = TextureManager::LoadTexture("res/BGTEXTURE.png");
    title_texture = TextureManager::LoadTexture("res/SIDEBAR_TITLE.png");
    
    digit_texture = TextureManager::LoadTexture("res/SIDEBAR_NUMBER.png");
    powerngraze_texture = TextureManager::LoadTexture("res/SIDEBAR_POWER.png");
    hp_texture = TextureManager::LoadTexture("res/SIDEBAR_HP.png");
    player_texture = TextureManager::LoadTexture("res/SIDEBAR_PLAYER.png");
}
Sidebar::~Sidebar() {
    // Free the textures
    if (bg_texture) {
        SDL_DestroyTexture(bg_texture);
        bg_texture = nullptr;
    }
    if (title_texture) {
        SDL_DestroyTexture(title_texture);
        title_texture = nullptr;
    }
    if (digit_texture) {
        SDL_DestroyTexture(digit_texture);
        digit_texture = nullptr;
    }
    if (powerngraze_texture) {
        SDL_DestroyTexture(powerngraze_texture);
        powerngraze_texture = nullptr;
    }
    if (hp_texture) {
        SDL_DestroyTexture(hp_texture);
        hp_texture = nullptr;
    }
    if (player_texture) {
        SDL_DestroyTexture(player_texture);
        player_texture = nullptr;
    }
    if (power_texture) {
        SDL_DestroyTexture(power_texture);
        player_texture = nullptr;
    }
    if (graze_texture) {
        SDL_DestroyTexture(graze_texture);
        graze_texture = nullptr;
    }

}

void Sidebar::render(int winW, int winH, Player* player) {
    //render background
    int w, h;
    SDL_QueryTexture(bg_texture, nullptr, nullptr, &w, &h); // get texture info
    tileSize = w;
    for (int y = 0; y < winH; y += tileSize) {
        for (int x = 0; x < winW; x += tileSize) {
            if ((x < tileSize * 2) || (x > winW - tileSize * 14)) {   // loop for render "side"bar
                destRect = { x, y, tileSize, tileSize };

                SDL_RenderCopy(Game::Grenderer, bg_texture, nullptr, &destRect); 
            }

        }
    }
    
    for (int y = 0; y < winH; y += tileSize) {
        for (int x = 0; x < winW; x += tileSize) {
            if ((y < tileSize) || (y > winH - tileSize * 2)) {      // loop for render "top&bot"bar
                destRect = { x ,y , tileSize, tileSize };

                SDL_RenderCopy(Game::Grenderer, bg_texture, nullptr, &destRect); 
            }
        }
    }

    render_score(Game::PLAYSCORE, 0);
    render_playerhp(player);

	// render title
	int TITLE_W, TITLE_H;
   	SDL_QueryTexture(title_texture, nullptr, nullptr, &TITLE_W, &TITLE_H);
	desRect_title = { 910, 590, TITLE_W * 2, TITLE_H * 2 };

	SDL_RenderCopy(Game::Grenderer, title_texture, nullptr, &desRect_title);   

     // render player health and power
    int POWER_W, POWER_H;
    SDL_QueryTexture(powerngraze_texture, nullptr, nullptr, &POWER_W, &POWER_H);
    destRect_power = { 910 , 280, POWER_W * 2, POWER_H * 2 };

    SDL_RenderCopy(Game::Grenderer, powerngraze_texture, nullptr, &destRect_power);
    
    int PLAYER_W, PLAYER_H;
    SDL_QueryTexture(player_texture, nullptr, nullptr, &PLAYER_W, &PLAYER_H);
    destRect_power = { 910 , 230, PLAYER_W * 2, PLAYER_H * 2 };

    SDL_RenderCopy(Game::Grenderer, player_texture, nullptr, &destRect_power);
}

void Sidebar::render_score(int score, int Highscore) {
    std::ostringstream scoreStream;
    std::ostringstream HighscoreStream;

    scoreStream << std::setw(SCORE_DIGITS) << std::setfill('0') << score;
    HighscoreStream << std::setw(SCORE_DIGITS) << std::setfill('0') << Highscore;

    std::string scoreStr = scoreStream.str();
    std::string HighscoreStr = HighscoreStream.str();

    for (size_t i = 0; i < scoreStr.length(); i++) {
        char digit = scoreStr[i] - '0';
        srcRect_digit = { digit * DIGIT_SIZE, 0, DIGIT_SIZE, DIGIT_SIZE };
        destRect_digit = { 985 + static_cast<int>(i * DIGIT_SIZE * 2) , 100, DIGIT_SIZE * 2, DIGIT_SIZE * 2 };

        SDL_RenderCopy(Game::Grenderer, digit_texture, &srcRect_digit, &destRect_digit);
    }

    for (size_t i = 0; i < HighscoreStr.length(); i++) {
        char digit = HighscoreStr[i] - '0';
        srcRect_digit = { digit * DIGIT_SIZE, 0, DIGIT_SIZE, DIGIT_SIZE };
        destRect_digit = { 985 + static_cast<int>(i * DIGIT_SIZE * 2) , 70, DIGIT_SIZE * 2, DIGIT_SIZE * 2 };

        SDL_RenderCopy(Game::Grenderer, digit_texture, &srcRect_digit, &destRect_digit);
    }
}

void Sidebar::render_playerhp(Player* player) {
    std::ostringstream grazeStream;
    std::ostringstream powerStream;

    grazeStream << std::setw(4) << std::setfill(' ') << player->getGraze();
    powerStream << std::fixed << std::setprecision(2) << player->getPlayerpowerlv();

    std::string grazeStr = grazeStream.str();
    std::string powerStr = powerStream.str();

    SDL_Color white = { 255, 255 ,255 };
    TTF_Font* font0 = TTF_OpenFont("res/DFPPOPCorn-W12.ttf", 16);

    graze_texture = TextureManager::LoadFontTexture(grazeStr.c_str(), font0, white);
    SDL_Rect grazeRect = { 1010, 323, 50, 20 };
    SDL_RenderCopy(Game::Grenderer, graze_texture, NULL, &grazeRect);

    power_texture = TextureManager::LoadFontTexture(powerStr.c_str(), font0, white);
    SDL_Rect powerRect = { 1010, 285, 60, 20 }; 
    SDL_RenderCopy(Game::Grenderer, power_texture, NULL, &powerRect);  

    if (player->getPlayerhp() > 0) {
        int HP_W, HP_H;
        SDL_QueryTexture(hp_texture, nullptr, nullptr, &HP_W, &HP_H);
        for (int i = 0; i < player->getPlayerhp() - 1; i++) {
            destRect_hp = { 1010 + (i * HP_W * 3 / 2 ), 230, HP_W * 3 / 2, HP_H * 3 / 2};
            SDL_RenderCopy(Game::Grenderer, hp_texture, nullptr, &destRect_hp);
        }
    }
}
