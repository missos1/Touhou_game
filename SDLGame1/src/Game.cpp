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
        player->handleInput(event); // pass event to player
        if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
            player->playerShoot(bullets); // bullets shoot
        }
    }
}

void Game::update() {
    int winW, winH;
    SDL_GetRendererOutputSize(renderer, &winW, &winH);
    player->update(); // update movement

    for (size_t i = 0; i < bullets.size(); i++) {
        bullets[i]->update();                       // bullets update
        if (bullets[i]->getY() < 0) {
            bullets.erase(bullets.begin() + i);
            i--;
        }
    }

}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    player->render(); // render player

    int winW, winH;
    SDL_GetRendererOutputSize(renderer, &winW, &winH);

    sidebar->render(winW, winH); // render sidebar
    for (Bullet* bullet : bullets) {
        bullet->render();
    }
    SDL_RenderPresent(renderer);
   
}

void Game::clean() {
    delete player; // free mem
    delete sidebar; 
    for (Bullet* bullet : bullets) {
        delete bullet; 
    }
    bullets.clear();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
