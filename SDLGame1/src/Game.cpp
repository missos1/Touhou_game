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
#include <iostream>
#define endl "\n"

SDL_Renderer* Game::Grenderer = nullptr; // Define the static renderer
GameState Game::state = GameState::LOADING; // Initial game state
GameState Game::prevState = GameState::LOADING; // Previous game state

Uint32 Game::GameStartTime = 0; // Game start time

int Game::PLAYSCORE = 0; // Player score

SDL_Texture* Game::Misc_player_text = nullptr; // Declare textures
SDL_Texture* Game::enemybullet_text = nullptr;
SDL_Texture* Game::Enemy_texture_w = nullptr;
SDL_Texture* Game::Enemy_texture_r = nullptr;
SDL_Texture* Game::Enemy_texture_b = nullptr;
SDL_Texture* Game::Enemy_texture_sparkle = nullptr;
SDL_Texture* Game::Menu_texture = nullptr;

Game::Game()
    : window(nullptr), isRunning(false), player(nullptr), // Initialize game variables
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

    player = new Player(410, 680); // Create player
    if (!player) {
        std::cout << "Player Creation Failed! Error: " << SDL_GetError() << endl;
        return false;
    }

    Mix_AllocateChannels(64); // Allocate audio channels

    if (!initText() || !initSM()) { // Initialize textures and sound manager
        return false;
    }

    MENU = new Menu(); // Create menu
    if (!MENU) {
        std::cout << "Menu Creation Failed! Error: " << SDL_GetError() << endl;
        return false;
    }

    isRunning = true; // Set isRunning to true if initialization is successful
    return true;
}

bool Game::initText() {
    Misc_player_text = TextureManager::LoadTexture("res/player/Reimu_sprite.png"); // Load player texture
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
    if (!SoundManager::LoadSound("graze", "res/sound/graze.wav")) return false;
    if (!SoundManager::LoadSound("plshoot", "res/sound/plst00.wav")) return false;
    if (!SoundManager::LoadSound("enshoot0", "res/sound/tan00.wav")) return false;
    if (!SoundManager::LoadSound("enshoot1", "res/sound/tan01.wav")) return false;
    if (!SoundManager::LoadSound("enshoot2", "res/sound/tan02.wav")) return false;
    if (!SoundManager::LoadSound("endie0", "res/sound/enep00.wav")) return false;
    if (!SoundManager::LoadSound("endie1", "res/sound/enep01.wav")) return false;
    if (!SoundManager::LoadSound("entakedmg", "res/sound/damage00.wav")) return false;
    if (!SoundManager::LoadSound("select", "res/sound/select00.wav")) return false;
    if (!SoundManager::LoadSound("ok", "res/sound/ok00.wav")) return false;
    if (!SoundManager::LoadSound("cancel", "res/sound/cancel00.wav")) return false;
    if (!SoundManager::LoadSound("collect_item", "res/sound/item00.wav")) return false;

    if (!SoundManager::LoadMusic("Mainmenu", "res/OST/A Dream that is more Scarlet than Red.mp3")) return false; // Load music
    if (!SoundManager::LoadMusic("Stage_theme", "res/OST/The Maid and the Pocket Watch of Blood.mp3")) return false;
    if (!SoundManager::LoadMusic("Boss_theme", "res/OST/Lunar Clock ~ Luna Dial.mp3")) return false;

    return true;
}

void Game::run() {
    int allocatedChannels = Mix_AllocateChannels(-1); // Allocate audio channels
    std::cout << "Currently allocated channels: " << allocatedChannels << endl;

    while (isRunning) { // Main game loop
        frameStart = SDL_GetTicks(); // Get start time of the frame

        if (state == GameState::LOADING && SDL_GetTicks() >= 2000) { // Transition from loading to menu state
            state = GameState::MENU;
        }
        handleEvents(); // Handle input events
        update(); // Update game state
        render(); // Render game state

        if (Game::prevState != GameState::MENU && Game::state == GameState::MENU) {
            SoundManager::PlayMusic("Mainmenu", -1, 255); // Play menu music
        }

        if (Game::prevState == GameState::MENU && Game::state == GameState::PLAYING) {
            Game::GameStartTime = SDL_GetTicks(); // Store the start time
            SoundManager::StopMusic(); // Stop menu music
            SoundManager::PlayMusic("Stage_theme", -1, 32); // Play stage music
            //std::cout << "Game started at: " << Game::GameStartTime << " ms" << endl;
        }

        Game::prevState = Game::state; // Update previous state

        frameTime = SDL_GetTicks() - frameStart; // Calculate frame time
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime); // Delay to maintain 60 FPS
        }
    }
}

void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) { // If the user closes the window
            isRunning = false;  // Set isRunning to false
        }
    }

    const Uint8* keys = SDL_GetKeyboardState(NULL); // Get keyboard state

    if (state == GameState::MENU) {
        MENU->handleInput(keys); // Handle menu input
    }

    else if (state == GameState::PLAYING) {
        player->handleInput(keys); // Handle player input

        static Uint32 lastShotTime = 0;
        Uint32 currentTime = SDL_GetTicks();
        if (keys[SDL_SCANCODE_SPACE] && currentTime - lastShotTime > 90) { // Handle shooting
            int powerlv = 0;
            player->playerShoot(player_bullets);
            //player->testshoot(player_bullets);
            lastShotTime = currentTime;
        }
    }

    else if (state == GameState::EXIT) isRunning = false; // Exit game
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
        items[i]->update();
    }

    //EnemyLayout::spawnHorizontal(enemies, -30, 300, 4, EnemyType::RED_FA,enemy_bullets, player);
    EnemyLayout::wave1(enemies, enemy_bullets, player); // Execute enemy wave
    //EnemyLayout::spawnVerticalWave(enemies, 5);
    CollisionCheck::EnemyColli(player_bullets, enemies, items, player); // Check collisions with enemies
    CollisionCheck::PlayerColli(enemy_bullets, player, items); // Check collisions with player
}

void Game::update() {
    if (state == GameState::PLAYING) {
        static int prevscore = 0;
        if (PLAYSCORE > prevscore) {
            std::cout << "playscore: " << PLAYSCORE << endl; // Debug: print player score
            prevscore = PLAYSCORE;
        }
        player->update(); // Update player
        ObjHandling(); // Update game objects
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

    else if (state == GameState::PLAYING) {
        player->render(); // Render player

        for (Bullet* bullet : player_bullets) {
            bullet->render(); // Render player bullets
        }

        for (Item* item : items) {
            item->render(); // Render items
        }

        for (Enemy* enemy : enemies) {
            enemy->render(); // Render enemies
        }

        for (Bullet* bullet : enemy_bullets) {
            bullet->render(); // Render enemy bullets
        }

        sidebar->render(winW, winH, player); // Render sidebar
    }

    SDL_RenderPresent(Grenderer); // Present the renderer
}

void Game::clean() {
    delete MENU; // Clean up menu
    delete player; // Clean up player
    delete sidebar; // Clean up sidebar
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
    enemies.clear(); // Clear enemy vector
    player_bullets.clear(); // Clear player bullet vector
    enemy_bullets.clear(); // Clear enemy bullet vector
    items.clear(); // Clear item vector
    TextureManager::cleanup(); // Clean up textures
    SoundManager::Clean(); // Clean up sounds
    SDL_DestroyRenderer(Grenderer); // Destroy renderer
    Mix_CloseAudio(); // Close audio
    SDL_DestroyWindow(window); // Destroy window
    SDL_Quit(); // Quit SDL
}


