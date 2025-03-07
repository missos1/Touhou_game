#include "headers/Game.hpp"
#include "headers/Bullets.hpp"
#include "headers/Player.hpp"
#include <iostream>
#define endl "\n"

Game::  Game()
    : window(nullptr), renderer(nullptr), isRunning(false), player(nullptr),
    frameStart(0), frameTime(0), sidebar(nullptr) {}

Game::~Game() {
    clean();
}

bool Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) {
    int flags = fullscreen ? SDL_WINDOW_FULLSCREEN : 0;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        std::cout << "SDL Initialization Failed! Error: " << SDL_GetError() << endl;  // initialize SDL with audio
        return false;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) { 
        std::cout << "SDL_mixer Initialization Failed! Error: " << Mix_GetError() << endl; // initialize mixer
        return false;
    }

    window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
    if (!window) {
        std::cout << "Window Creation Failed! Error: " << SDL_GetError() << endl;   // create window
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cout << "Renderer Creation Failed! Error: " << SDL_GetError() << endl; // create renderer
        return false;
    }

    sidebar = new Sidebar(renderer);
    if (!sidebar) {
        std::cout << "Background Creation Failed! Error: " << SDL_GetError() << endl;  // render sidebar
        return false;
    }

    player = new Player(renderer, 410, 680); // create player
    isRunning = true;
    return true;

}

void Game::run() {
    // temporary music test
    Mix_Music* bgMusic = Mix_LoadMUS("res/OST/A Dream that is more Scarlet than Red.mp3");
    if (!bgMusic) {
        std::cout << "Failed to load music! Error: " << Mix_GetError() << endl;
    }

    Mix_PlayMusic(bgMusic, -1);

    // game loop
    while (isRunning) { 
        frameStart = SDL_GetTicks();

        handleEvents();
        update();
        render();


        frameTime = SDL_GetTicks() - frameStart;     // fps limiter
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }
}

void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            isRunning = false;
        }
    }

    const Uint8* keys = SDL_GetKeyboardState(NULL); // keyboard input
    player->handleInput(keys);

    static Uint32 lastShotTime = 0;
    Uint32 currentTime = SDL_GetTicks();
    if (keys[SDL_SCANCODE_SPACE] && currentTime - lastShotTime > 100) {
        player->playerShoot(player_bullets); // shooting call
        lastShotTime = currentTime;
    }
}

void Game::update() {
    int winW, winH;
    SDL_GetRendererOutputSize(renderer, &winW, &winH);
    player->update(); // update movement


    for (size_t i = 0; i < player_bullets.size(); i++) {
        player_bullets[i]->update();                       // bullets update
        if (player_bullets[i]->getY() < 0) {
            player_bullets.erase(player_bullets.begin() + i);
            i--;
        }
    }

}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    int winW, winH;
    SDL_GetRendererOutputSize(renderer, &winW, &winH);

    for (Bullet* bullet : player_bullets) {
        bullet->render();
    }
    player->render(); // render player
    sidebar->render(winW, winH); // render sidebar

    SDL_RenderPresent(renderer);
}

void Game::clean() {
    delete player; // free mem
    delete sidebar; 
    for (Bullet* bullet : player_bullets) {
        delete bullet; 
    }
    player_bullets.clear();
    SDL_DestroyRenderer(renderer);
    Mix_CloseAudio();
    SDL_DestroyWindow(window);
    SDL_Quit();
}
