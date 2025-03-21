#include "headers/Menu.hpp"
#include "headers/Game.hpp"
#include "headers/TextureManager.hpp"
#include "headers/SoundManager.hpp"

Menu::Menu() : bg { 0, 0, 0, 0 }, selectedOption(0) {
	background = TextureManager::LoadTexture("res/Menu/Menu_bg.png");
	QuitSL = TextureManager::LoadTexture("res/Menu/Quit.png");
	QuitUSL = TextureManager::LoadTexture("res/Menu/Quit_unsl.png");
	StartSL = TextureManager::LoadTexture("res/Menu/Start.png");
	StartUSL = TextureManager::LoadTexture("res/Menu/Start_unsl.png");

	bg = { 0,0, 640, 480 };
	Quit = { 0, 0, 65, 30 };
	Start = { 0, 0, 75, 30 };
	
}

Menu::~Menu() {}

void Menu::handleInput(const Uint8* keys) {
    static bool keyPressed = false;

    if (!keyPressed) {
        if (keys[SDL_SCANCODE_S]) {
            selectedOption = (selectedOption + 1) % 2; // Toggle between 0 and 1
            SoundManager::PlaySound("select", 0, 32);
            keyPressed = true;
        }
        if (keys[SDL_SCANCODE_W]) {
            selectedOption = (selectedOption - 1 + 2) % 2; // Wrap around
            SoundManager::PlaySound("select", 0, 32);
            keyPressed = true;
        }
        if (keys[SDL_SCANCODE_ESCAPE]) {
            selectedOption = 1; // Wrap around
            SoundManager::PlaySound("cancel", 0, 32);
            keyPressed = true;
        }
        if (keys[SDL_SCANCODE_RETURN] || keys[SDL_SCANCODE_SPACE]) {
            if (selectedOption == 0) {
                Game::state = GameState::PLAYING; // Start the game
                Game::prevState = GameState::MENU;
                SoundManager::PlaySound("ok", 0, 128);
            }
            else {
                Game::state = GameState::EXIT; // Quit game
                SoundManager::PlaySound("cancel", 0, 32);
            }
            keyPressed = true;
        }

    }

    if (!keys[SDL_SCANCODE_W] && !keys[SDL_SCANCODE_S] && !keys[SDL_SCANCODE_RETURN] && !keys[SDL_SCANCODE_ESCAPE]) {
        keyPressed = false; // Reset key press state
    }
}

void Menu::render() {
	SDL_Rect bgDest = { 0, 0, 640 * 2, 480 * 2 };
	SDL_Rect StartDest = { 950, 420, 75 * 2, 30 * 2 };
	SDL_Rect QuitDest = { 920, 500, 65 * 2, 30 * 2 };
	SDL_RenderCopy(Game::Grenderer, background, &bg, &bgDest);


	/*SDL_RenderCopy(Game::Grenderer, StartUSL, &Quit, &StartDest);
	SDL_RenderCopy(Game::Grenderer, QuitUSL, &Quit, &QuitDest);*/

    SDL_RenderCopy(Game::Grenderer, (selectedOption == 0) ? StartSL : StartUSL, &Start, &StartDest);
    /* Render Quit button based on selection*/
    SDL_RenderCopy(Game::Grenderer, (selectedOption == 1) ? QuitSL : QuitUSL, &Quit, &QuitDest);
}