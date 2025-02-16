#include "Game.h"
#include <SDL_image.h>

Game* game = nullptr;

int main(int argc, char* argv[]) {
    game = new Game();

    if (!game->init("SDL2 Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920, 1080, false)) {
        return -1;
    }

    while (game->running()) {
        game->handleEvents();
        game->update();
        game->render();
    }

    game->clean();
    delete game;
    return 0;
}
