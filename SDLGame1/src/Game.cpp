#include "headers/Game.hpp"
#include "headers/Bullets.hpp"
#include "headers/Player.hpp"
#include <iostream>
#define endl "\n";

Game::Game()
    : window(nullptr), renderer(nullptr), isRunning(false), player(nullptr),
    frameStart(0), frameTime(0), sidebar(nullptr) {}

Game::~Game() {
    clean();
}

bool Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) {
    int flags = fullscreen ? SDL_WINDOW_FULLSCREEN : 0;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL Initialization Failed! Error: " << SDL_GetError() << endl;
        return false;
    }

    window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
    if (!window) {
        std::cout << "Window Creation Failed! Error: " << SDL_GetError() << endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cout << "Renderer Creation Failed! Error: " << SDL_GetError() << endl;
        return false;
    }

    sidebar = new Sidebar(renderer);
    if (!sidebar) {
        std::cout << "Background Creation Failed! Error: " << SDL_GetError() << endl;
        return false;
    }

    player = new Player(renderer, 410, 680); // create player
    isRunning = true;
    return true;
}

void Game::run() {
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

    const Uint8* keys = SDL_GetKeyboardState(NULL);
    player->handleInput(keys);

    if (keys[SDL_SCANCODE_SPACE]) {
        if (SDL_GetTicks() % 100 < 32) {
            player->playerShoot(player_bullets);
        }
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
    SDL_DestroyWindow(window);
    SDL_Quit();
}
