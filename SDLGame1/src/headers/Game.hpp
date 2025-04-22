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
const int PLAY_AREA_X_MIN = 30;
const int PLAY_AREA_X_MAX = 840;
const int PLAY_AREA_Y_MIN = 10;
const int PLAY_AREA_Y_MAX = 855;
const int PLAYER_HB_SIZE = 10;
const int PLAYER_OG_X = 415;
const int PLAYER_OG_Y = 680;
const int BOSS_OG_X = -150;
const int BOSS_OG_Y = 120;

const SDL_Color white = { 255, 255, 255 };
const SDL_Color red = { 255, 0, 0 };
const SDL_Color light_gray = { 200, 200, 200 };
const SDL_Color very_light_gray = { 230, 230, 230 };
const SDL_Color gray = { 128, 128, 128 };
const SDL_Color dark_gray = { 50, 50, 50 };
const SDL_Color yellow = { 255, 255, 0 };
const SDL_Color pink = { 255, 100, 100 };

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
    RESTARTING,
    EXIT,
};

class Game {
public:
    Game();
    ~Game();

    static TTF_Font* font0;
    static TTF_Font* font1;
    static TTF_Font* font2;

	static SDL_Renderer* Grenderer;
    static GameState state;
    static GameState prevState;
    static Uint64 GameStartTime;
    static Uint64 GamecurrentTime;
	static Uint64 GamePauseStartTime;
    static Uint64 GameExitTime;
    static Uint64 GamePauseTotalTime;

    static SDL_Texture* Misc_player_text;
    static SDL_Texture* enemybullet_text;
    static SDL_Texture* Enemy_texture_w;
    static SDL_Texture* Enemy_texture_r;
    static SDL_Texture* Enemy_texture_b;
    static SDL_Texture* Enemy_texture_sparkle;
    static SDL_Texture* Menu_texture;

	static int BGM_volume;
	static int SE_volume;
    
    static int PLAYSCORE;

    bool init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
    void ObjHandling();
    bool initText();
    bool initSM();
    void handleEvents();
    void pauseGame(const SDL_Event& event);
    void stateHandling();
    void resetObject();
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
    const Uint64 frameDelay = 1000 / FPS;  // frame stuff
    Uint64 frameStart;
    Uint64 frameTime;
};

#endif
