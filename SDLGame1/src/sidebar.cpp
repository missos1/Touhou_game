#include "headers/sidebar.hpp"
#include "headers/Game.hpp"
#include "headers/Player.hpp"
#include "headers/SoundManager.hpp"
#include "headers/Boss.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

Sidebar::Sidebar()
    : tileSize(0), frame_texture(nullptr), title_texture(nullptr), text_texture(nullptr),
    destRect{ 0, 0, 0, 0 }, destRect_hp{ 0, 0, 0, 0 },destRect_boss_hp { 0, 0, 0 ,0 }, selectedOption(0) {

    frame_texture = TextureManager::LoadTexture("res/Sidebar/FRAMETEXTURE.png");
    title_texture = TextureManager::LoadTexture("res/Sidebar/SIDEBAR_TITLE.png");
    powerngraze_texture = TextureManager::LoadTexture("res/Sidebar/SIDEBAR_POWER.png");
    hp_texture = TextureManager::LoadTexture("res/Sidebar/SIDEBAR_HP.png");
    player_texture = TextureManager::LoadTexture("res/Sidebar/SIDEBAR_PLAYER.png");
    background_texture = TextureManager::LoadTexture("res/Sidebar/Background.png");
	boss_HP_texture = TextureManager::LoadTexture("res/Sidebar/SIDEBAR_BOSS_HP.png");
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

}

void Sidebar::render(int winW, int winH, Player* player, Boss* boss) {
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

    render_score(Game::PLAYSCORE, Game::HIGHSCORE);
    render_playerhp(player);
    render_boss_hud(boss);

	// render title

    TextureManager::render_from_texture(title_texture, 910, 590, 2, 0, SDL_FLIP_NONE);
    TextureManager::render_from_texture(powerngraze_texture, 910, 280, 2, 0, SDL_FLIP_NONE);
    TextureManager::render_from_texture(player_texture, 910, 230, 2, 0, SDL_FLIP_NONE);

    if (Game::state != GameState::PAUSE) return;

    render_pausescreen();
    
}

void Sidebar::render_background() {
    int w, h;
    SDL_QueryTexture(background_texture, nullptr, nullptr, &w, &h);
    SDL_SetTextureAlphaMod(background_texture, 90);

    static std::vector<int> yPos = { 0, 0 - h * 3 };
    
    //SDL_Rect bg_destRect_0 = { 45, 20, w * 2, h * 2};
    //SDL_Rect bg_destRect_1 = { 10, yPos + h * 2, w * 2, h * 2 };

    for (int& i : yPos) {
        SDL_Rect bg_destRect= { 10, i, w * 3, h * 3 };
        SDL_RenderCopy(Game::Grenderer, background_texture, nullptr, &bg_destRect);
        if (Game::state != GameState::PAUSE) i += 5;
        if (i >= h * 3) i = 0 - h * 3;
    }
}

void Sidebar::handleInputs_pausescreen(const SDL_Event& event) {
    if (event.type != SDL_KEYDOWN) return;

    switch (event.key.keysym.sym) {
    case SDLK_s:
        selectedOption = (selectedOption + 1) % 3; // Toggle between 0 and 1
        SoundManager::PlaySound("select", 0, Game::SE_volume);
        break;
    case SDLK_w:
        selectedOption = (selectedOption - 1 + 3) % 3; // Wrap around
        SoundManager::PlaySound("select", 0, Game::SE_volume);
        break;
    case SDLK_ESCAPE:
        selectedOption = 2; // Select ESC
        SoundManager::PlaySound("cancel", 0, Game::SE_volume);
        break;
    case SDLK_RETURN:
    case SDLK_SPACE:
        if (selectedOption == 0) {           
            Game::state = GameState::PLAYING; // Resume the game
	        Game::prevState = GameState::PAUSE; // Set previous state to PAUSE
            Game::GamePauseTotalTime += SDL_GetTicks64() - Game::GamePauseStartTime; 
        }
        else if (selectedOption == 1) {
            Game::state = GameState::RESTARTING; // Restart the game
            Game::prevState = GameState::PLAYING; // Set previous state to PAUSE
			Game::GameStartTime = SDL_GetTicks64(); // Store the start time
			Game::GamePauseTotalTime = 0; // Reset pause time
            Game::PLAYSCORE = 0; // Reset score
        }
        else {
            Game::state = GameState::MENU; // Goes back to menu
			Game::prevState = GameState::PLAYING; // Set previous state to PLAYING
            Game::GameStartTime = 0; // Reset time
            Game::GamePauseTotalTime = 0; // Reset time
            Game::PLAYSCORE = 0; // Reset score
        }
        SoundManager::PlaySound("ok", 0, Game::SE_volume);
        break;
    default:
        selectedOption = 0;
        break;
    }
}

void Sidebar::handleInputs_wonscreen(const SDL_Event& event) {
    if (event.type != SDL_KEYDOWN) return;


}

// Private functions below

void Sidebar::render_score(int score, int Highscore) {
    std::ostringstream scoreStream, HighscoreStream;
    scoreStream << std::setw(SCORE_DIGITS) << std::setfill('0') << score;
    HighscoreStream << std::setw(SCORE_DIGITS) << std::setfill('0') << Highscore;

    std::string scoreStr = scoreStream.str();
    std::string HighscoreStr = HighscoreStream.str();

    TextureManager::render_text("Score", Game::font0, light_gray, text_texture, 890, 136);
    TextureManager::render_text("Hi-Score", Game::font0, light_gray, text_texture, 890, 100);
    TextureManager::render_text(scoreStr, Game::font1, white, text_texture, 1000, 136);
    TextureManager::render_text(HighscoreStr, Game::font1, white, text_texture, 1000, 100);
}

void Sidebar::render_playerhp(Player* player) {
    std::ostringstream grazeStream;
    std::ostringstream powerStream;

    grazeStream << std::setw(4) << std::setfill(' ') << player->getGraze();
    powerStream << std::fixed << std::setprecision(2) << player->getPlayerpowerlv();

    std::string grazeStr = grazeStream.str();
    std::string powerStr = powerStream.str();

    TextureManager::render_text(grazeStr, Game::font0, white, text_texture, 1010, 320);
    TextureManager::render_text(powerStr, Game::font0, white, text_texture, 1010, 285);

    if (player->getPlayerhp() > 0) {
        int HP_W, HP_H;
        SDL_QueryTexture(hp_texture, nullptr, nullptr, &HP_W, &HP_H);
        for (int i = 0; i < player->getPlayerhp() - 1; i++) {
            destRect_hp = { 1010 + (i * HP_W * 3 / 2 ), 230, HP_W * 3 / 2, HP_H * 3 / 2};
            SDL_RenderCopy(Game::Grenderer, hp_texture, nullptr, &destRect_hp);
        }
    }
}

void Sidebar::render_boss_hud(Boss* boss) {
    if (boss->getPhase() == Phase::IDLE || boss->getPhase() == Phase::DEAD) return;
    
    render_bosshp(boss);
    render_boss_time(boss);
    render_boss_spellcard_count(boss);
	render_boss_name(boss);
}

void Sidebar::render_bosshp(Boss* boss) {
    SDL_Rect srcRect = { 0, 8, 16, 8 };
    destRect_boss_hp = { 240, 50, BOSS_HPBAR_W , 8 };

    double hp_percent = boss->getBosshp_percent();

    destRect_boss_hp.w = static_cast<int>(BOSS_HPBAR_W * hp_percent);

	if (boss->isphase_spellcard()) SDL_SetTextureColorMod(boss_HP_texture, 255, 100, 100);
	else SDL_SetTextureColorMod(boss_HP_texture, 255, 255, 255);

    SDL_RenderCopy(Game::Grenderer, boss_HP_texture, &srcRect, &destRect_boss_hp);
}

void Sidebar::render_boss_time(Boss* boss) {
	int time_left = boss->getTimeleft();
    std::string time_left_str = std::to_string((time_left > 99) ? 99 : time_left);
    TextureManager::render_text((time_left < 10) ? '0' + time_left_str : time_left_str, Game::font1, (time_left <= 5) ? red : light_gray, text_texture, 805, 45); // Display phase time left
    static int prevtime_left = 99;
    if (time_left <= 5 && time_left != prevtime_left) {
		prevtime_left = time_left;
		SoundManager::PlaySound("timeout", 0, Game::SE_volume);
    }
}

void Sidebar::render_boss_name(Boss* boss) {
	std::string name = "Flandre";
	TextureManager::render_text(name, Game::font0, yellow, text_texture, 80, 40);
}

void Sidebar::render_boss_spellcard_count(Boss* boss){
	std::string spellcard_count = std::to_string(boss->getBoss_spellcardcount());
	TextureManager::render_text('0' + spellcard_count, Game::font0, light_gray, text_texture, 190, 40); // Display spellcard count
}

void Sidebar::render_pausescreen() {
    SDL_SetRenderDrawBlendMode(Game::Grenderer, SDL_BLENDMODE_BLEND);
    SDL_Rect opaque = { 64, 32, 800, 896 };
    SDL_SetRenderDrawColor(Game::Grenderer, 10, 200, 255, 50); // Grey
    SDL_RenderFillRect(Game::Grenderer, &opaque);

    TextureManager::render_text("PAUSE", Game::font1, gray, text_texture, 400, 390);
    TextureManager::render_text("UNPAUSE", Game::font0, (selectedOption == 0) ? red : white, text_texture, 405, 440);
    TextureManager::render_text("RESTART", Game::font0, (selectedOption == 1) ? red : white, text_texture, 407, 470);
    TextureManager::render_text("EXIT", Game::font0, (selectedOption == 2) ? red : white, text_texture, 440, 500);
}

