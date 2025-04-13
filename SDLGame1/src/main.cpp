#include "headers/Game.hpp"

Game* game = nullptr;

int main(int argc, char* argv[]) {
    srand(static_cast<unsigned>(time(0)));
    game = new Game();

    if (!game->init("Touhou_6_alpha", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIN_WIDTH, WIN_HEIGHT, false)) {
        return -1;
    }

    game->run();

    game->clean();
    return 0;
}
