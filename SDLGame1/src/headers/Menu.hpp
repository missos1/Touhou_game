#ifndef MENU_HPP
#define MENU_HPP

#include <SDL.h>
#include <SDL_image.h>

class Game;
class TextureManager;
enum class MenuState {
	TITLE,
	MAIN,
	OPTION,
};

class Menu {
public:
	Menu();
	~Menu();

	void handleInput(const SDL_Event& event);
	void render();

private:
	void render_title();
	void render_option();
	void handleInput_option(const SDL_Event& event);

	MenuState state;
	SDL_Texture* title_texture;
	SDL_Texture* background;
	SDL_Texture* QuitSL;
	SDL_Texture* QuitUSL;
	SDL_Texture* StartSL;
	SDL_Texture* StartUSL;
	SDL_Texture* OptionSL;
	SDL_Texture* OptionUSL;
	SDL_Texture* BGMSL;
	SDL_Texture* BGMUSL;
	SDL_Texture* SoundSL;
	SDL_Texture* SoundUSL;
	SDL_Texture* text_texture;

	int selectedOption;
	int selectedOption_1;

	const int TITLE_X_1 = 120;
	const int START_X_1 = 990;
	const int OPTION_X_1 = 965;
	const int QUIT_X_1 = 935;

	const int BGM_X = 350;
	const int SOUND_X = 350;
	const int QUIT_X_2 = 350;
	const int BGM_Y = 350;
	const int SOUND_Y = 470;
	const int QUIT_Y_1 = 580;

	const int TITLE_Y = 20;
	const int START_Y = 420;
	const int OPTION_Y = 500;
	const int QUIT_Y = 580;

	const int TITLE_X_0 = 465;
	const int START_X_0 = 1350;
	const int OPTION_X_0 = 1320;
	const int QUIT_X_0 = 1280;
};

#endif