#include "headers/Game.hpp"
#include "headers/Bullets.hpp"
#include "headers/Player.hpp"
#include "headers/Enemy.hpp"
#include "headers/EnemyLayout.hpp"
#include "headers/TextureManager.hpp"
#include "headers/SoundManager.hpp"
#include "headers/Collision.hpp"
#include "headers/Menu.hpp"
#include "headers/Items.hpp"
#include "headers/Boss.hpp"
#include "headers/ScoreManager.hpp"
#include <iostream>
#define endl "\n"

SDL_Renderer* Game::Grenderer = nullptr; // Define the static renderer
GameState Game::state = GameState::LOADING; // Initial game state
GameState Game::prevState = GameState::LOADING; // Previous game state

TTF_Font* Game::font0 = nullptr; // Font for text rendering
TTF_Font* Game::font1 = nullptr; // Font for text rendering
TTF_Font* Game::font2 = nullptr; // Font for text rendering

int Game::BGM_volume = 32; // Initial BGM volume
int Game::SE_volume = 32; // Initial SE volume 

double Game::PlayerPowerLV = 0.0;
int Game::PlayerHP = 0;

Uint64 Game::GameStartTime = 0; // Game start time
//Uint64 Game::GamePauseTime
Uint64 Game::GameExitTime = 0; // Game exit time
Uint64 Game::GamePauseStartTime = 0; // pausing's variable handler

Uint64 Game::GamePauseTotalTime = 0; // Game pause total time
Uint64 Game::GamecurrentTime = 0; // Game pause time

int Game::PLAYSCORE = 0; // Player score
int Game::HIGHSCORE = 0; // Player score

SDL_Texture* Game::Misc_player_text = nullptr; // Declare textures
SDL_Texture* Game::enemybullet_text = nullptr;
SDL_Texture* Game::Enemy_texture_w = nullptr;
SDL_Texture* Game::Enemy_texture_r = nullptr;
SDL_Texture* Game::Enemy_texture_b = nullptr;
SDL_Texture* Game::Enemy_texture_sparkle = nullptr;
SDL_Texture* Game::Menu_texture = nullptr;

Game::Game()
    : window(nullptr), isRunning(false), player(nullptr), boss(nullptr), // Initialize game variables
    frameStart(0), frameTime(0), sidebar(nullptr), MENU(nullptr) {
}

Game::~Game() {
    clean(); // Clean up resources
}

bool Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) {
    int flags = fullscreen ? SDL_WINDOW_FULLSCREEN : 0; // Set fullscreen flag

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        std::cout << "SDL Initialization Failed! Error: " << SDL_GetError() << endl; // Initialize SDL with video and audio
        return false;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cout << "SDL_mixer Initialization Failed! Error: " << Mix_GetError() << endl; // Initialize SDL_mixer
        return false;
    }

    if (TTF_Init() < 0) {
        std::cout << "SDL_ttf Initialization Failed! Error: " << TTF_GetError() << endl;
    }

    window = SDL_CreateWindow(title, xpos, ypos, width, height, flags); // Create window
    if (!window) {
        std::cout << "Window Creation Failed! Error: " << SDL_GetError() << endl;
        return false;
    }

    Grenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED); // Create renderer
    if (!Grenderer) {
        std::cout << "Renderer Creation Failed! Error: " << SDL_GetError() << endl;
        return false;
    }

    sidebar = new Sidebar(); // Create sidebar
    if (!sidebar) {
        std::cout << "Sidebar Creation Failed! Error: " << SDL_GetError() << endl;
        return false;
    }

    player = new Player(PLAYER_OG_X, PLAYER_OG_Y); // Create player
    if (!player) {
        std::cout << "Player Creation Failed! Error: " << SDL_GetError() << endl;
        return false;
    }

    boss = new Boss(BOSS_OG_X, BOSS_OG_Y);

    Mix_AllocateChannels(16); // Allocate audio channels

    if (!initText() || !initSM()) { // Initialize textures and sound manager
        return false;
    }

    MENU = new Menu(); // Create menu
    if (!MENU) {
        std::cout << "Menu Creation Failed! Error: " << SDL_GetError() << endl;
        return false;
    }

    font0 = TTF_OpenFont("res/DFPPOPCorn-W12.ttf", 24);
    font1 = TTF_OpenFont("res/DFPPOPCorn-W12.ttf", 36);
    font2 = TTF_OpenFont("res/DFPPOPCorn-W12.ttf", 48);

	if (!font0 || !font1) {
		std::cout << "Font Loading Failed! Error: " << TTF_GetError() << endl; // Load fonts
		return false;
	}

    isRunning = true; // Set isRunning to true if initialization is successful
    return true;
}

bool Game::initText() {
	Misc_player_text = TextureManager::LoadTexture("res/player/Reimu_sprite.png"); // Load player bullet texture
    if (!Misc_player_text) return false;

    enemybullet_text = TextureManager::LoadTexture("res/bullets.png"); // Load enemy bullet texture
    if (!enemybullet_text) return false;

    Enemy_texture_w = TextureManager::LoadTexture("res/Enemy/White_fa.png"); // Load enemy textures
    if (!Enemy_texture_w) return false;

    Enemy_texture_r = TextureManager::LoadTexture("res/Enemy/Red_fa.png");
    if (!Enemy_texture_r) return false;

    Enemy_texture_b = TextureManager::LoadTexture("res/Enemy/Blue_fa.png");
    if (!Enemy_texture_b) return false;

    Enemy_texture_sparkle = TextureManager::LoadTexture("res/Enemy/Sparkle.png");
    if (!Enemy_texture_sparkle) return false;

    return true;
}

bool Game::initSM() {
    if (!SoundManager::LoadSound("pldead", "res/sound/pldead00.wav")) return false; // Load sounds
    if (!SoundManager::LoadSound("plheal", "res/sound/extend.wav")) return false; 
    if (!SoundManager::LoadSound("graze", "res/sound/graze.wav")) return false;
    if (!SoundManager::LoadSound("plshoot", "res/sound/plst00.wav")) return false;
    if (!SoundManager::LoadSound("enshoot0", "res/sound/tan00.wav")) return false;
    if (!SoundManager::LoadSound("enshoot1", "res/sound/tan01.wav")) return false;
    if (!SoundManager::LoadSound("endie0", "res/sound/enep00.wav")) return false;
    if (!SoundManager::LoadSound("endie1", "res/sound/enep01.wav")) return false;
    if (!SoundManager::LoadSound("enshoot2", "res/sound/tan02.wav")) return false;
    if (!SoundManager::LoadSound("entakedmg", "res/sound/damage00.wav")) return false;
    if (!SoundManager::LoadSound("select", "res/sound/select00.wav")) return false;
    if (!SoundManager::LoadSound("ok", "res/sound/ok00.wav")) return false;
    if (!SoundManager::LoadSound("cancel", "res/sound/cancel00.wav")) return false;
    if (!SoundManager::LoadSound("collect_item", "res/sound/item00.wav")) return false;
    if (!SoundManager::LoadSound("enemypowerup0", "res/sound/power0.wav")) return false;
    if (!SoundManager::LoadSound("enemypowerup1", "res/sound/power1.wav")) return false;
    if (!SoundManager::LoadSound("pl_powerup", "res/sound/powerup.wav")) return false;
    if (!SoundManager::LoadSound("kira0", "res/sound/kira00.wav")) return false;
    if (!SoundManager::LoadSound("kira1", "res/sound/kira01.wav")) return false;
    if (!SoundManager::LoadSound("kira2", "res/sound/kira02.wav")) return false;
	if (!SoundManager::LoadSound("timeout", "res/sound/timeout.wav")) return false;

    if (!SoundManager::LoadMusic("Mainmenu", "res/OST/A Dream that is more Scarlet than Red.mp3")) return false; // Load music
    if (!SoundManager::LoadMusic("Stage_theme", "res/OST/The Centennial Festival for Magical Girls.mp3")) return false;
    if (!SoundManager::LoadMusic("Boss_theme", "res/OST/U.N. Owen was Her.mp3")) return false;

    return true;
}

void Game::run() {
    int allocatedChannels = Mix_AllocateChannels(-1); // Allocate audio channels
    std::cout << "Currently allocated channels: " << allocatedChannels << endl;

    while (running()) { // Main game loop

        frameStart = SDL_GetTicks64(); // Get start time of the frame


        handleEvents(); // Handle input events
        update(); // Update game state
        render(); // Render game state

        stateHandling();
        
        frameTime = SDL_GetTicks64() - frameStart; // Calculate frame time
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime); // Delay to maintain 60 FPS
        }
    }
}

void Game::stateHandling() {
    if (state == GameState::LOADING && SDL_GetTicks64() >= 2000) { // Transition from loading to menu state
        state = GameState::MENU;
        ScoreManager::readScorefromfile("res/Score/Score.txt"); // Read high score from file
    }

    if (Game::prevState != GameState::MENU && Game::state == GameState::MENU) {
        SoundManager::PlayMusic("Mainmenu", -1, Game::BGM_volume); // Play menu music
		ScoreManager::writeScoretofile("res/Score/Score.txt"); // Write score to file
		ScoreManager::readScorefromfile("res/Score/Score.txt"); // Read high score from file
        resetObject();
    }

    if (Game::prevState == GameState::PLAYING && Game::state == GameState::RESTARTING) {
		state = GameState::PLAYING; // Restart game
        ScoreManager::readScorefromfile("res/Score/Score.txt"); // Read high score from file
        resetObject();
    }

    if (Game::prevState != GameState::PAUSE && Game::state == GameState::PAUSE) {
        SoundManager::PauseMusic(); // Pause music
    }
    else if (Game::prevState == GameState::PAUSE && Game::state == GameState::PLAYING) {
        SoundManager::ResumeMusic(); // Resume music
    }

    if (Game::prevState == GameState::MENU && Game::state == GameState::PLAYING) {
        SoundManager::StopMusic();
        //std::cout << "Game started at: " << Game::GameStartTime << " ms" << endl;
    }

    Game::prevState = Game::state; // Update previous state
}

void Game::handleEvents() {

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) { // If the user closes the window
            isRunning = false;  // Set isRunning to false
        }

        pauseGame(event);

        if (state == GameState::MENU) {
            MENU->handleInput(event); // Handle menu input
        }
        else if (state == GameState::PAUSE) {
            sidebar->handleInputs_pausescreen(event);
        }
    }

	const Uint8* keys = SDL_GetKeyboardState(nullptr); // Get the current state of the keyboard

    if (state == GameState::PLAYING) {
        player->handleInput(keys, player_bullets); // Handle player input
        //boss->debug_ani(keys); // debug boss animation
    }

    else if (state == GameState::EXIT && SDL_GetTicks64() - GameExitTime >= 200) isRunning = false; // Exit game
}

void Game::pauseGame(const SDL_Event& event) {
    if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_ESCAPE && state == GameState::PLAYING) {
            state = GameState::PAUSE;
            GamePauseStartTime = SDL_GetTicks64();
            prevState = GameState::PLAYING;
			//std::cout << "Game paused at: " << GamePauseStartTime << " ms" << endl;
        }
    }
}

void Game::ObjHandling() {
    for (int i = (int)enemy_bullets.size() - 1; i >= 0; i--) { // Update enemy bullets
        enemy_bullets[i]->update();
    }

    for (int i = (int)player_bullets.size() - 1; i >= 0; i--) { // Update player bullets
        player_bullets[i]->update();
    }

    for (int i = (int)enemies.size() - 1; i >= 0; i--) { // Update enemies
        enemies[i]->update();
    }

    for (int i = (int)items.size() - 1; i >= 0; i--) { // Update items
        items[i]->update(player);
    }

    //EnemyLayout::spawnHorizontal(enemies, -30, 300, 4, EnemyType::RED_FA,enemy_bullets, player);
    EnemyLayout::stage(enemies, enemy_bullets, player, boss); // Execute stage
    //EnemyLayout::spawnVerticalWave(enemies, 5);
}

void Game::update() {
    if (state == GameState::PLAYING) {

        //static int prevscore = 0;
        ////if (PLAYSCORE > prevscore) {
        ////    std::cout << "playscore: " << PLAYSCORE << endl; // Debug: print player score
        ////    prevscore = PLAYSCORE;
        ////}
        boss->update(enemy_bullets, items);
        player->update(); // Update player
        ObjHandling(); // Update game objects
        CollisionCheck::EnemyColli(player_bullets, enemies, items, player); // Check collisions with enemies
        CollisionCheck::PlayerColli(enemy_bullets, player, items); // Check collisions with player
        CollisionCheck::BossColli(player_bullets, boss, items); // Check collisions with boss
        CollisionCheck::DeleleOffScreen(enemy_bullets, player_bullets, enemies, items); // Deleting objects offscreen
		if (Game::PLAYSCORE > Game::HIGHSCORE) {
			Game::HIGHSCORE = Game::PLAYSCORE; // Update high score
		}
    }
}

void Game::render() {
    SDL_SetRenderDrawColor(Grenderer, 0, 0, 0, 255); // Set render draw color to black
    SDL_RenderClear(Grenderer); // Clear the renderer

    int winW, winH;
    SDL_GetRendererOutputSize(Grenderer, &winW, &winH); // Get window size

    if (state == GameState::LOADING) {
        SDL_Texture* loading = TextureManager::LoadTexture("res/Menu/Loading.png"); // Load loading texture
        SDL_Rect src_load = { 0,0, 640, 480 };
        SDL_Rect dest_load = { 0,0, 640 * 2, 480 * 2 };

        SDL_RenderCopy(Grenderer, loading, &src_load, &dest_load); // Render loading screen
    }

    else if (state == GameState::MENU) {
        MENU->render(); // Render menu
    }

    else if (state == GameState::PLAYING || state == GameState::PAUSE) {

        sidebar->render_background();

        boss->render();

        for (Item* item : items) {
            item->render(); // Render items
        }

        for (Bullet* bullet : player_bullets) {
            bullet->render(); // Render player bullets
        }

        for (Enemy* enemy : enemies) {
            enemy->render(); // Render enemies
        }

        player->render(); // Render player

        for (Bullet* bullet : enemy_bullets) {
            bullet->render(); // Render enemy bullets
        }

        sidebar->render(winW, winH, player, boss); // Render sidebar
    }

    SDL_RenderPresent(Grenderer); // Present the renderer
}

void Game::clean() {
    delete MENU; // Clean up menu
	MENU = nullptr; // Set menu to nullptr
    delete boss;
	boss = nullptr; // Set boss to nullptr
    delete player; // Clean up player
	player = nullptr; // Set player to nullptr
    delete sidebar; // Clean up sidebar
	sidebar = nullptr; // Set sidebar to nullptr
    for (Bullet* bullet : player_bullets) {
        delete bullet; // Clean up player bullets
    }
    for (Bullet* bullet : enemy_bullets) {
        delete bullet; // Clean up enemy bullets
    }
    for (Enemy* enemy : enemies) {
        delete enemy; // Clean up enemies
    }
    for (Item* item : items) {
        delete item; // Clean up items
    }
    TTF_CloseFont(font0); // Close fonts
    TTF_CloseFont(font1);
    TTF_CloseFont(font2);
    enemies.clear(); // Clear enemy vector
    player_bullets.clear(); // Clear player bullet vector
    enemy_bullets.clear(); // Clear enemy bullet vector
    items.clear(); // Clear item vector
    TextureManager::cleanup(); // Clean up textures
    SoundManager::Clean(); // Clean up sounds
    SDL_DestroyRenderer(Grenderer); // Destroy renderer
    Mix_CloseAudio(); // Close audio
    TTF_Quit(); // Close font
    SDL_DestroyWindow(window); // Destroy window
    SDL_Quit(); // Quit SDL
}

void Game::resetObject() {
    for (Bullet* bullet : player_bullets) {
        delete bullet;
    }
    player_bullets.clear();

    for (Bullet* bullet : enemy_bullets) {
        delete bullet;
    }
    enemy_bullets.clear();

    for (Enemy* enemy : enemies) {
        delete enemy;
    }
    enemies.clear();

    for (Item* item : items) {
        delete item;
    }
    items.clear();

    if (boss) boss->resetValue();
    if (player) player->resetValue();
}


