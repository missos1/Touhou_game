#include "headers/Game.hpp"
#include "headers/Bullets.hpp"
#include "headers/Player.hpp"
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
        lastShotTime = currentTime;
    }
}

void Game::update() {
    int winW, winH;
    SDL_GetRendererOutputSize(Grenderer, &winW, &winH);
	player->update(); // update player

    for (size_t i = 0; i < player_bullets.size(); i++) {
        player_bullets[i]->update();
        if (player_bullets[i]->getY() < 0) {
			player_bullets.erase(player_bullets.begin() + i); // remove bullets that go off screen
            i--;
        }
    }
}

void Game::render() {
    SDL_SetRenderDrawColor(Grenderer, 0, 0, 0, 255);
    SDL_RenderClear(Grenderer);

    int winW, winH;
	SDL_GetRendererOutputSize(Grenderer, &winW, &winH); // get window size

    for (Bullet* bullet : player_bullets) {
		bullet->render(); // render bullets
    }
	player->render(); // render player
	sidebar->render(winW, winH); // render sidebar

    SDL_RenderPresent(Grenderer); 
}

void Game::clean() {
    delete player;
    delete sidebar;
    for (Bullet* bullet : player_bullets) {
		delete bullet; // clean up
    }
    player_bullets.clear();
    SDL_DestroyRenderer(Grenderer);
    Mix_CloseAudio();
    SDL_DestroyWindow(window);
    SDL_Quit();
}
