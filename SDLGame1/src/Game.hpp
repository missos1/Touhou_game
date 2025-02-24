#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include "Player.hpp"
#include "sidebar.hpp"

class Game {
public:
    Game();
    ~Game();

    bool init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
    void handleEvents();
    void update();
    void render();
    void clean();
    void run();

    bool running() const { return isRunning; }

private:
    bool isRunning;
    SDL_Window* window;
    SDL_Renderer* renderer;

    Player* player;

    Sidebar* sidebar;

    const int FPS = 60;
    const int frameDelay = 1000 / FPS;  // frame stuff
    Uint32 frameStart;
    int frameTime;
};

#endif
