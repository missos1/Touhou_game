#ifndef GAME_HPP
#define GAME_HPP

#include <SDL.h>
#include <vector>
#include <SDL_mixer.h>
#include "sidebar.hpp"

const int WIN_WIDTH = 1280;
const int WIN_HEIGHT = 980;

class Bullet;
class Player;
class Enemy;    
class TextureManager;
class SoundManager;
class CollisionCheck;

class Game {
public:
    Game();
    ~Game();

	static SDL_Renderer* Grenderer;

    static SDL_Texture* Misc_player_text;
    static SDL_Texture* enemybullet_text;
    static SDL_Texture* Enemy_texture_w;
    static SDL_Texture* Enemy_texture_r;
    static SDL_Texture* Enemy_texture_b;

    bool init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
    void ObjHandling();
    void initText();
    void initSM();
    void handleEvents();
    void update();
    void render();
    void clean();
    void run();

    bool running() const { return isRunning; }

private:
    bool isRunning;
    SDL_Window* window;

    Player* player;
    std::vector<Enemy*> enemies; 

    std::vector<Bullet*> player_bullets;
    std::vector<Bullet*> enemy_bullets;

    Sidebar* sidebar;

    const int FPS = 60;
    const int frameDelay = 1000 / FPS;  // frame stuff
    Uint32 frameStart;
    int frameTime;
};

#endif
