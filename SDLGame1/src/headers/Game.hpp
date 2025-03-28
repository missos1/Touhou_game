#ifndef GAME_HPP
#define GAME_HPP

#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <ctime>
#include <random>
#include <vector>

#include "sidebar.hpp"

const int WIN_WIDTH = 1280;
const int WIN_HEIGHT = 960;
const int PLAY_AREA_X_MIN = 20;
const int PLAY_AREA_X_MAX = 820;
const int PLAY_AREA_Y_MIN = 10;
const int PLAY_AREA_Y_MAX = 855;
const int PLAYER_HB_SIZE = 10;

class Bullet;
class Player;
class Enemy;    
class TextureManager;
class SoundManager;
class CollisionCheck;
class Menu;
class Item;
class Boss;

enum class GameState {
    LOADING,
    MENU,
    PLAYING,
    PAUSE,
    EXIT,
};

class Game {
public:
    Game();
    ~Game();

	static SDL_Renderer* Grenderer;
    static GameState state;
    static GameState prevState;
    static Uint32 GameStartTime;

    static SDL_Texture* Misc_player_text;
    static SDL_Texture* enemybullet_text;
    static SDL_Texture* Enemy_texture_w;
    static SDL_Texture* Enemy_texture_r;
    static SDL_Texture* Enemy_texture_b;
    static SDL_Texture* Enemy_texture_sparkle;
    static SDL_Texture* Menu_texture;
    
    static int PLAYSCORE;
    static bool isTimeStopped;

    bool init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
    void ObjHandling();
    bool initText();
    bool initSM();
    void handleEvents();
    void update();
    void render();
    void clean();
    void run();

    bool running() const { return isRunning; }

private:


    bool isRunning;
    SDL_Window* window;

    Menu* MENU;

    Player* player;
    Boss* boss;
    std::vector<Enemy*> enemies; 

    std::vector<Bullet*> player_bullets;
    std::vector<Bullet*> enemy_bullets;
    std::vector<Item*> items;

    Sidebar* sidebar;

    const int FPS = 60;
    const int frameDelay = 1000 / FPS;  // frame stuff
    Uint32 frameStart;
    int frameTime;

    
};

#endif
