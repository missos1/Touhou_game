#include "headers/Game.hpp"
#include "headers/Bullets.hpp"
#include "headers/Player.hpp"
#include "headers/Enemy.hpp"
#include "headers/EnemyLayout.hpp"
#include "headers/TextureManager.hpp"
#include "headers/SoundManager.hpp"
#include "headers/Collision.hpp"
#include <iostream>
#define endl "\n"

SDL_Renderer* Game::Grenderer = nullptr; // define the static renderer

SDL_Texture* Game::Misc_player_text = nullptr; // declare texture
SDL_Texture* Game::enemybullet_text = nullptr;
SDL_Texture* Game::Enemy_texture_w = nullptr;
SDL_Texture* Game::Enemy_texture_r = nullptr;
SDL_Texture* Game::Enemy_texture_b = nullptr;

Game::Game()
    : window(nullptr), isRunning(false), player(nullptr), // game variables
    frameStart(0), frameTime(0), sidebar(nullptr) {
}

Game::~Game() {
    clean(); // clean funct
}

bool Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) {
    int flags = fullscreen ? SDL_WINDOW_FULLSCREEN : 0; // fullscreen or not

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		std::cout << "SDL Initialization Failed! Error: " << SDL_GetError() << endl; // init sdl with audio and video
        return false;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		std::cout << "SDL_mixer Initialization Failed! Error: " << Mix_GetError() << endl; // init sdl mixer
        return false;
    }

    window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
    if (!window) {
		std::cout << "Window Creation Failed! Error: " << SDL_GetError() << endl; // create window
        return false;
    }

    Grenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!Grenderer) {
		std::cout << "Renderer Creation Failed! Error: " << SDL_GetError() << endl; // create renderer
        return false;
    }

    sidebar = new Sidebar();
    if (!sidebar) {
		std::cout << "Sidebar Creation Failed! Error: " << SDL_GetError() << endl; // create sidebar
        return false;
    }

    player = new Player(410, 680);
    if (!player) {
		std::cout << "Player Creation Failed! Error: " << SDL_GetError() << endl; // create player
        return false;
    }

    Game::initText();
    Game::initSM();

	isRunning = true; // if everything is successful, set isRunning to true
    return true;
}

void Game::initText() {
    Misc_player_text = TextureManager::LoadTexture("res/player/Reimu_sprite.png"); // texture
    enemybullet_text = TextureManager::LoadTexture("res/bullets.png");
    Enemy_texture_w = TextureManager::LoadTexture("res/Enemy/White_fa.png");
    Enemy_texture_r = TextureManager::LoadTexture("res/Enemy/Red_fa.png");
    Enemy_texture_b = TextureManager::LoadTexture("res/Enemy/Blue_fa.png");;

}

void Game::initSM() {
    SoundManager::LoadSound("pldead", "res/sound/pldead00.wav");
    SoundManager::LoadSound("graze", "res/sound/graze.wav");
    SoundManager::LoadSound("plshoot", "res/sound/plst00.wav");
    SoundManager::LoadSound("enshoot0", "res/sound/tan00.wav");
    SoundManager::LoadSound("enshoot1", "res/sound/tan01.wav");
    SoundManager::LoadSound("enshoot2", "res/sound/tan02.wav");
    SoundManager::LoadSound("endie0", "res/sound/enep00.wav");
    SoundManager::LoadSound("endie1", "res/sound/enep01.wav");
    SoundManager::LoadSound("entakedmg", "res/sound/damage00.wav");
    
    SoundManager::LoadMusic("Mainmenu", "res/OST/A Dream that is more Scarlet than Red.mp3");
}

void Game::run() {
    SoundManager::PlayMusic("Mainmenu", -1, 255);

	while (isRunning) { // main game loop
        frameStart = SDL_GetTicks();

        handleEvents();
        update();
        render();

		frameTime = SDL_GetTicks() - frameStart; // frame locked at 60fps
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }
}

void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) { // if the user closes the window
            isRunning = false;
        }
    }

    const Uint8* keys = SDL_GetKeyboardState(NULL);
	player->handleInput(keys); //   handle player input

    static Uint32 lastShotTime = 0;
    Uint32 currentTime = SDL_GetTicks();
	if (keys[SDL_SCANCODE_SPACE] && currentTime - lastShotTime > 100) { // shooting
        int powerlv = 0;
        player->playerShoot(player_bullets);
        //player->testshoot(player_bullets);
        lastShotTime = currentTime;
    }
}

void Game::ObjHandling() {
    //std::cout << player->getX() << " " << player->getY();
    static int grazecount = 0;


    for (int i = (int)enemy_bullets.size() - 1; i >= 0; i--) { // update enemies' bullets
        enemy_bullets[i]->update();
    }

    for (int i = (int)player_bullets.size() - 1; i >= 0; i--) { // update player's bullets
        player_bullets[i]->update();
    }

    for (int i = (int)enemies.size() - 1; i >= 0; i--) { // update enemies
        enemies[i]->update();
    }


    //EnemyLayout::spawnHorizontal(enemies, -30, 300, 4, EnemyType::RED_FA,enemy_bullets, player);
    EnemyLayout::wave1(enemies, enemy_bullets, player);
    //EnemyLayout::spawnVerticalWave(enemies, 5);
    CollisionCheck::EnemyColli(player_bullets, enemies);
    CollisionCheck::PlayerColli(enemy_bullets, player);
}

void Game::update() {
    int winW, winH;
    SDL_GetRendererOutputSize(Grenderer, &winW, &winH);
	player->update(); // update player
    ObjHandling();
}

void Game::render() {
    SDL_SetRenderDrawColor(Grenderer, 0, 0, 0, 255); // render black window
    SDL_RenderClear(Grenderer);

    int winW, winH;
	SDL_GetRendererOutputSize(Grenderer, &winW, &winH); // get window size

    for (Bullet* bullet : player_bullets) {
		bullet->render(); // render bullets
    }

    for (Enemy* enemy : enemies) {
        enemy->render(); // render enemies
    }

    for (Bullet* bullet : enemy_bullets) {
        bullet->render(); // render bullets
    }

	player->render(); // render player
	sidebar->render(winW, winH); // render sidebar

    SDL_SetRenderDrawColor(Game::Grenderer, 128, 0, 128, 255); // Purple

    SDL_RenderPresent(Grenderer); // final drawing
}

void Game::clean() {
    delete player;
    delete sidebar;
    for (Bullet* bullet : player_bullets) {
		delete bullet; // clean up
    }
    for (Bullet* bullet : enemy_bullets) {
        delete bullet; // clean up
    }
    for (Enemy* enemy : enemies) {
        delete enemy; // clean up
    }
    enemies.clear();
    player_bullets.clear();
    TextureManager::cleanup();
    SoundManager::Clean();
    SDL_DestroyRenderer(Grenderer);
    Mix_CloseAudio();
    SDL_DestroyWindow(window);
    SDL_Quit();
}
