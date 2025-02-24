#include "Game.hpp"
#include <SDL_image.h>

Game* game = nullptr;

int main(int argc, char* argv[]) {
    game = new Game();

    if (!game->init("Touhou", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 960, false)) {
        return -1;
    }

    game->run();

    game->clean();
    return 0;
}
