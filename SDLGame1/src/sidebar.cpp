#include "headers/sidebar.hpp"
#include "headers/Game.hpp"
#include "headers/Player.hpp"
#include "headers/SoundManager.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

Sidebar::Sidebar()
    : tileSize(0), frame_texture(nullptr), title_texture(nullptr), text_texture(nullptr),
    destRect{ 0, 0, 0, 0 }, destRect_hp{ 0, 0, 0, 0}, selectedOption(0) {

    frame_texture = TextureManager::LoadTexture("res/FRAMETEXTURE.png");
    title_texture = TextureManager::LoadTexture("res/SIDEBAR_TITLE.png");
    powerngraze_texture = TextureManager::LoadTexture("res/SIDEBAR_POWER.png");
    hp_texture = TextureManager::LoadTexture("res/SIDEBAR_HP.png");
    player_texture = TextureManager::LoadTexture("res/SIDEBAR_PLAYER.png");
    font0 = TTF_OpenFont("res/DFPPOPCorn-W12.ttf", 24);
    font1 = TTF_OpenFont("res/DFPPOPCorn-W12.ttf", 36);
}
Sidebar::~Sidebar() {
    // Free the textures
    if (frame_texture) {
        SDL_DestroyTexture(frame_texture);
        frame_texture = nullptr;
    }
    if (title_texture) {
        SDL_DestroyTexture(title_texture);
        title_texture = nullptr;
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

    if (font0) {
        TTF_CloseFont(font0);
        font0 = nullptr;
    }
    if (font1) {
        TTF_CloseFont(font1);
        font1 = nullptr;
    }
}

void Sidebar::render(int winW, int winH, Player* player) {
    //render background
    int w, h;
    SDL_QueryTexture(frame_texture, nullptr, nullptr, &w, &h); // get texture info
    tileSize = w;
    for (int y = 0; y < winH; y += tileSize) {
        for (int x = 0; x < winW; x += tileSize) {
            if ((x < tileSize * 2) || (x > winW - tileSize * 14)) {   // loop for render "side"bar
                destRect = { x, y, tileSize, tileSize };

                SDL_RenderCopy(Game::Grenderer, frame_texture, nullptr, &destRect); 
            }

        }
    }
    
    for (int y = 0; y < winH; y += tileSize) {
        for (int x = 0; x < winW; x += tileSize) {
            if ((y < tileSize) || (y > winH - tileSize * 2)) {      // loop for render "top&bot"bar
                destRect = { x ,y , tileSize, tileSize };

                SDL_RenderCopy(Game::Grenderer, frame_texture, nullptr, &destRect); 
            }
        }
    }

    render_score(Game::PLAYSCORE, 0);
    render_playerhp(player);

	// render title

    TextureManager::render_from_texture(title_texture, 910, 590, 2, 0, SDL_FLIP_NONE);
    TextureManager::render_from_texture(powerngraze_texture, 910, 280, 2, 0, SDL_FLIP_NONE);
    TextureManager::render_from_texture(player_texture, 910, 230, 2, 0, SDL_FLIP_NONE);

    if (Game::state == GameState::PAUSE) {
        render_pausescreen();
    }
}


void Sidebar::handleInputs_pausescreen(const SDL_Event& event) {
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
        case SDLK_s:
            selectedOption = (selectedOption + 1) % 2; // Toggle between 0 and 1
            SoundManager::PlaySound("select", 0, Game::SE_volume);
            break;
        case SDLK_w:
            selectedOption = (selectedOption - 1 + 2) % 2; // Wrap around
            SoundManager::PlaySound("select", 0, Game::SE_volume);
            break;
        case SDLK_ESCAPE:
            selectedOption = 1; // Select ESC
            SoundManager::PlaySound("cancel", 0, Game::SE_volume);
            break;
        case SDLK_RETURN:
        case SDLK_SPACE:
            if (selectedOption == 0) {           
                Game::state = GameState::PLAYING; // Start the game
                Game::prevState = GameState::PAUSE;
                Game::GamePauseTotalTime += SDL_GetTicks() - Game::GamePauseStartTime;
                SoundManager::PlaySound("ok", 0, Game::SE_volume);
            }
            else {
                Game::GameStartTime = 0; // Reset time
                Game::GamePauseTotalTime = 0; // Reset time
                Game::PLAYSCORE = 0; // Reset score
                Game::state = GameState::MENU; // Goes back to menu
                Game::prevState = GameState::PLAYING;

                SoundManager::PlaySound("ok", 0, Game::SE_volume);
            }
            break;
        default:
            break;
        }
    }
}

// Private functions below

void Sidebar::render_score(int score, int Highscore) {
    std::ostringstream scoreStream, HighscoreStream;
    scoreStream << std::setw(SCORE_DIGITS) << std::setfill('0') << score;
    HighscoreStream << std::setw(SCORE_DIGITS) << std::setfill('0') << Highscore;

    std::string scoreStr = scoreStream.str();
    std::string HighscoreStr = HighscoreStream.str();

    TextureManager::render_text("Score", font0, light_gray, text_texture, 890, 136);
    TextureManager::render_text("Hi-Score", font0, light_gray, text_texture, 890, 100);
    TextureManager::render_text(scoreStr, font1, white, text_texture, 1020, 136);
    TextureManager::render_text(HighscoreStr, font1, white, text_texture, 1020, 100);
}

void Sidebar::render_playerhp(Player* player) {
    std::ostringstream grazeStream;
    std::ostringstream powerStream;

    grazeStream << std::setw(4) << std::setfill(' ') << player->getGraze();
    powerStream << std::fixed << std::setprecision(2) << player->getPlayerpowerlv();

    std::string grazeStr = grazeStream.str();
    std::string powerStr = powerStream.str();

    TextureManager::render_text(grazeStr, font0, white, text_texture, 1010, 320);
    TextureManager::render_text(powerStr, font0, white, text_texture, 1010, 285);

    if (player->getPlayerhp() > 0) {
        int HP_W, HP_H;
        SDL_QueryTexture(hp_texture, nullptr, nullptr, &HP_W, &HP_H);
        for (int i = 0; i < player->getPlayerhp() - 1; i++) {
            destRect_hp = { 1010 + (i * HP_W * 3 / 2 ), 230, HP_W * 3 / 2, HP_H * 3 / 2};
            SDL_RenderCopy(Game::Grenderer, hp_texture, nullptr, &destRect_hp);
        }
    }
}

void Sidebar::render_pausescreen() {
    SDL_SetRenderDrawBlendMode(Game::Grenderer, SDL_BLENDMODE_BLEND);
    SDL_Rect opaque = { 64, 32, 800, 896 };
    SDL_SetRenderDrawColor(Game::Grenderer, 10, 200, 255, 50); // Grey
    SDL_RenderFillRect(Game::Grenderer, &opaque);

    TextureManager::render_text("PAUSE", font1, dark_gray, text_texture, 400, 390);
    TextureManager::render_text("UNPAUSE", font0, (selectedOption == 0) ? red : white, text_texture, 405, 440);
    TextureManager::render_text("EXIT", font0, (selectedOption == 1) ? red : white, text_texture, 440, 470);
}

