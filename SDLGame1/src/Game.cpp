#include "headers/Game.hpp"
#include "headers/Bullets.hpp"
#include "headers/Player.hpp"
#include "headers/Enemy.hpp"
#include "headers/EnemyLayout.hpp"
#include <iostream>
#define endl "\n"

SDL_Renderer* Game::Grenderer = nullptr; // define the static renderer

Game::Game()
    : window(nullptr), isRunning(false), player(nullptr),
    frameStart(0), frameTime(0), sidebar(nullptr) {
}

Game::~Game() {
    clean();
}

bool Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) {
    int flags = fullscreen ? SDL_WINDOW_FULLSCREEN : 0;

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

	isRunning = true; // if everything is successful, set isRunning to true
    return true;
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

    for (int i = (int)player_bullets.size() - 1; i >= 0; i--) { // update player's bullets
        player_bullets[i]->update();
        if (player_bullets[i]->getY() < 0) {
            delete player_bullets[i];
            player_bullets.erase(player_bullets.begin() + i);
        }
    }

    for (int i = (int)enemy_bullets.size() - 1; i >= 0; i--) { // update enemies' bullets
        enemy_bullets[i]->update();
        if (enemy_bullets[i]->getY() < -100 ||
            enemy_bullets[i]->getY() > WIN_HEIGHT + 100 ||
            enemy_bullets[i]->getX() < -100 ||
            enemy_bullets[i]->getX() > WIN_WIDTH + 100) {
            delete enemy_bullets[i];
            enemy_bullets.erase(enemy_bullets.begin() + i);
        }
    }

    for (int i = (int)enemies.size() - 1; i >= 0; i--) { // update enemies
        enemies[i]->update();
        if (enemies[i]->getY() < -100 ||
            enemies[i]->getY() > 1000 ||
            enemies[i]->getX() > 1500 ||
            enemies[i]->getX() < -100) {
            delete enemies[i];
            enemies.erase(enemies.begin() + i);
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
        enemy->render();
    }

    for (Bullet* bullet : enemy_bullets) {
        bullet->render(); // render bullets
    }

	player->render(); // render player
	sidebar->render(winW, winH); // render sidebar

    SDL_SetRenderDrawColor(Game::Grenderer, 128, 0, 128, 255); // Purple

    SDL_RenderPresent(Grenderer); 
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
    SDL_DestroyRenderer(Grenderer);
    Mix_CloseAudio();
    SDL_DestroyWindow(window);
    SDL_Quit();
}
