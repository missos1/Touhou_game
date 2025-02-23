#include "Game.hpp"
#include <iostream>
#define endl "\n";

Game::Game() : window(nullptr), renderer(nullptr), isRunning(false), player(nullptr), frameStart(0), frameTime(0), bgtexture(nullptr), tileSize(0) {}

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


    bgtexture = TextureManager::LoadTexture("res/BGTEXTURE.png", renderer);
    if (!bgtexture) {
        std::cout << "Background Creation Failed! Error: " << SDL_GetError() << endl;
        return false;
    }

    int w, h;
    SDL_QueryTexture(bgtexture, nullptr, nullptr, &w, &h);
    tileSize = w;

    player = new Player(renderer, 140, 380); // create player
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
    }
}

void Game::update() {
    player->update(); // update movement
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    player->render(); // render player

    int winW, winH;
    SDL_GetRendererOutputSize(renderer, &winW, &winH);

    SDL_Rect destRect;
    for (int y = 0; y < winH; y += tileSize) {
        for (int x = 0; x < winW; x += tileSize) {
            if ((x < tileSize * 2) || (x > tileSize * 26)) {
                //if ((x < tileSize) || (x > tileSize * 5)) {
                destRect = { x ,y , tileSize, tileSize };
                SDL_RenderCopy(renderer, bgtexture, nullptr, &destRect); //render background
                //}
            }
        }
    }


    SDL_RenderPresent(renderer);
}

void Game::clean() {
    delete player; // free mem
    SDL_DestroyTexture(bgtexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
