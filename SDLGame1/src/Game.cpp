#include "headers/Game.hpp"
#include "headers/Bullets.hpp"
#include "headers/Player.hpp"
#include "headers/Enemy.hpp"
#include "headers/EnemyLayout.hpp"
#include "headers/TextureManager.hpp"
#include <iostream>
#define endl "\n"

SDL_Renderer* Game::Grenderer = nullptr; // define the static renderer

SDL_Texture* Game::Misc_player_text = nullptr; // declare texture
SDL_Texture* Game::enemybullet_text = nullptr;
SDL_Texture* Game::Enemy_texture_w = nullptr;
SDL_Texture* Game::Enemy_texture_r = nullptr;

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

	isRunning = true; // if everything is successful, set isRunning to true
    return true;
}

void Game::initText() {
    Misc_player_text = TextureManager::LoadTexture("res/player/Reimu_sprite.png"); // texture
    enemybullet_text = TextureManager::LoadTexture("res/bullets.png");
    Enemy_texture_w = TextureManager::LoadTexture("res/Enemy/White_fa.png");
    Enemy_texture_r = TextureManager::LoadTexture("res/Enemy/Red_fa.png");
}

void Game::run() {
    Mix_Music* bgMusic = Mix_LoadMUS("res/OST/A Dream that is more Scarlet than Red.mp3");
    if (!bgMusic) {
        std::cout << "Failed to load music! Error: " << Mix_GetError() << endl;
    }

    Mix_PlayMusic(bgMusic, -1);

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

void Game::update() {
    int winW, winH;
    SDL_GetRendererOutputSize(Grenderer, &winW, &winH);
	player->update(); // update player

    //std::cout << player->getX() << " " << player->getY();
    static int grazecount = 0;

    for (int i = (int)player_bullets.size() - 1; i >= 0; i--) { // update player's bullets

        player_bullets[i]->update();

        if (player_bullets[i]->getY() < 0) {
            delete player_bullets[i];
            player_bullets.erase(player_bullets.begin() + i);
        }
    }

    for (int i = (int)enemy_bullets.size() - 1; i >= 0; i--) { // update enemies' bullets
        enemy_bullets[i]->update();

        SDL_Rect bullet_hitbox = enemy_bullets[i]->getHitbox();
        SDL_Rect player_grazebox = player->getGrazingBox();
        SDL_Rect player_hitbox = player->getHitbox();

        if (SDL_HasIntersection(&player_grazebox, &bullet_hitbox) && !enemy_bullets[i]->getGrazeState()) { // grazing bullets for more points (future ver)
            enemy_bullets[i]->GrazeUpdate();
            std::cout << ++grazecount << endl;
        }

        if (SDL_HasIntersection(&player_hitbox, &bullet_hitbox)) { // hp collision
            delete enemy_bullets[i];
            enemy_bullets.erase(enemy_bullets.begin() + i);
            player->updatePlayerhp();
            std::cout << player->getPlayerhp() << endl;
            /*if (player->getPlayerhp() <= 0) {
                delete player; // crash game lmao
            }*/
        }

        if (enemy_bullets[i]->getY() < -100 ||
            enemy_bullets[i]->getY() > WIN_HEIGHT + 100 ||  // offscreen deletion
            enemy_bullets[i]->getX() < -100 ||
            enemy_bullets[i]->getX() > WIN_WIDTH + 100) {
            delete enemy_bullets[i];
            enemy_bullets.erase(enemy_bullets.begin() + i); // erase bullets
        }
    }

    for (int i = (int)enemies.size() - 1; i >= 0; i--) { // update enemies

        enemies[i]->update();

        if (enemies[i]->getY() < -100 ||
            enemies[i]->getY() > 1000 ||
            enemies[i]->getX() > 1500 ||
            enemies[i]->getX() < -100) {
            delete enemies[i];
            enemies.erase(enemies.begin() + i); // erase enemies
        }
    }
    EnemyLayout::spawnHorizontalWave(enemies, 30, enemy_bullets, player);
    EnemyLayout::spawnVerticalWave(enemies, 5);
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
    SDL_DestroyRenderer(Grenderer);
    Mix_CloseAudio();
    SDL_DestroyWindow(window);
    SDL_Quit();
}
