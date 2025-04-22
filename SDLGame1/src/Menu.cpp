#include "headers/Menu.hpp"
#include "headers/Game.hpp"
#include "headers/TextureManager.hpp"
#include "headers/SoundManager.hpp"

Menu::Menu() :  selectedOption(0), selectedOption_1(0), text_texture(nullptr), state(MenuState::TITLE) {
	title_texture = TextureManager::LoadTexture("res/Menu/Game_title.png");
	background = TextureManager::LoadTexture("res/Menu/Menu_bg.png");
	QuitSL = TextureManager::LoadTexture("res/Menu/Quit.png");
	QuitUSL = TextureManager::LoadTexture("res/Menu/Quit_unsl.png");
	StartSL = TextureManager::LoadTexture("res/Menu/Start.png");
	StartUSL = TextureManager::LoadTexture("res/Menu/Start_unsl.png");
	OptionSL = TextureManager::LoadTexture("res/Menu/Option.png");
	OptionUSL = TextureManager::LoadTexture("res/Menu/Option_unsl.png");
	BGMSL = TextureManager::LoadTexture("res/Menu/BGM.png");
	BGMUSL = TextureManager::LoadTexture("res/Menu/BGM_unsl.png");
	SoundSL = TextureManager::LoadTexture("res/Menu/Sound.png");
	SoundUSL = TextureManager::LoadTexture("res/Menu/Sound_unsl.png");
}

Menu::~Menu() { // Destructor
	if (background) {
		SDL_DestroyTexture(background);
		background = nullptr;
	}
	if (QuitSL) {
		SDL_DestroyTexture(QuitSL);
		QuitSL = nullptr;
	}
	if (QuitUSL) {
		SDL_DestroyTexture(QuitUSL);
		QuitUSL = nullptr;
	}
	if (StartSL) {
		SDL_DestroyTexture(StartSL);
		StartSL = nullptr;
	}
	if (StartUSL) {
		SDL_DestroyTexture(StartUSL);
		StartUSL = nullptr;
	}
	if (title_texture) {
		SDL_DestroyTexture(title_texture);
		title_texture = nullptr;
	}
	if (text_texture) {
		SDL_DestroyTexture(text_texture);
		text_texture = nullptr;
	}
	if (OptionSL) {
		SDL_DestroyTexture(OptionSL);
		OptionSL = nullptr;
	}
	if (OptionUSL) {
		SDL_DestroyTexture(OptionUSL);
		OptionUSL = nullptr;
	}
}

void Menu::handleInput(const SDL_Event& event) {
    if (event.type != SDL_KEYDOWN) return;

	if (state == MenuState::TITLE) {
		state = MenuState::MAIN;
		return;
	}

	if (state == MenuState::OPTION) {
		handleInput_option(event);
		return;
	}

    switch (event.key.keysym.sym) {
    case SDLK_s:
        selectedOption = (selectedOption + 1) % 3; // Toggle between 0 and 1
        SoundManager::PlaySound("select", 0, Game::SE_volume);
        break;
    case SDLK_w:
        selectedOption = (selectedOption - 1 + 3) % 3; // Wrap around
        SoundManager::PlaySound("select", 0, Game::SE_volume);
        break;
    case SDLK_ESCAPE:
        selectedOption = 1; // Select ESC
        SoundManager::PlaySound("cancel", 0, Game::SE_volume);
        break;
    case SDLK_RETURN:
    case SDLK_SPACE:
        if (selectedOption == 0) {
            Game::GameStartTime = SDL_GetTicks64(); // Store the start time            
            Game::state = GameState::PLAYING; // Start the game
            Game::prevState = GameState::MENU;

            SoundManager::PlaySound("ok", 0, Game::SE_volume);
        }
		else if (selectedOption == 1) {
			state = MenuState::OPTION; // Go to option menu
			SoundManager::PlaySound("ok", 0, Game::SE_volume);
		}
        else {
            Game::state = GameState::EXIT; // Quit game
            Game::GameExitTime = SDL_GetTicks64(); // Store the quit time

            SoundManager::PlaySound("cancel", 0, Game::SE_volume);
        }
        break;
    default:
        break;
    }
}

void Menu::handleInput_option(const SDL_Event& event) {
	if (event.type != SDL_KEYDOWN) return;

	switch (event.key.keysym.sym) {
	case SDLK_s:
		selectedOption_1 = (selectedOption_1 + 1) % 3; // Toggle between 0, 1, and 2
		SoundManager::PlaySound("select", 0, Game::SE_volume);
		break;
	case SDLK_w:
		selectedOption_1 = (selectedOption_1 - 1 + 3) % 3; // Wrap around
		SoundManager::PlaySound("select", 0, Game::SE_volume);
		break;
	case SDLK_a: // Decrease volume
		if (selectedOption_1 == 0 && Game::BGM_volume > 0) { // Adjust BGM volume
			Game::BGM_volume -= 4;
			if (Game::BGM_volume < 0) Game::BGM_volume = 0;
			SoundManager::SetVolumeMusic(Game::BGM_volume); // Apply the new BGM volume
		}
		else if (selectedOption_1 == 1 && Game::SE_volume > 0) { // Adjust SE volume
			Game::SE_volume -= 4;
			if (Game::SE_volume < 0) Game::SE_volume = 0;
		}
		SoundManager::PlaySound("select", 0, Game::SE_volume);
		break;
	case SDLK_d: // Increase volume
		if (selectedOption_1 == 0 && Game::BGM_volume < 255) { // Adjust BGM volume
			Game::BGM_volume += 4;
			if (Game::BGM_volume > 255) Game::BGM_volume = 255;
			SoundManager::SetVolumeMusic(Game::BGM_volume); // Apply the new BGM volume
		}
		else if (selectedOption_1 == 1 && Game::SE_volume < 255) { // Adjust SE volume
			Game::SE_volume += 4;
			if (Game::SE_volume > 255) Game::SE_volume = 255;
		}
		SoundManager::PlaySound("select", 0, Game::SE_volume);
		break;
	case SDLK_ESCAPE: // Return to the main menu
		selectedOption_1 = 2; // Select ESC
		SoundManager::PlaySound("cancel", 0, Game::SE_volume);
		break;
	case SDLK_RETURN:
	case SDLK_SPACE:
		if (selectedOption_1 == 2) {
			state = MenuState::MAIN; // Return to the main menu
			SoundManager::PlaySound("ok", 0, Game::SE_volume);
		}
	default:
		break;
	}
}

void Menu::render() {
	TextureManager::render_from_texture(background, 0, 0, 2, 0, SDL_FLIP_NONE);
	
	render_option();
	render_title();

	TextureManager::render_text("Hoang Minh Duc's Game Project", Game::font0, light_gray, text_texture, 170, 820);
	TextureManager::render_text("All rights belong to Team Shanghai Alice", Game::font0, light_gray, text_texture, 620, 820);
	TextureManager::render_text("All assests were taken from the spriters resource", Game::font0, gray, text_texture, 370, 920);
}

void Menu::render_option() {
	if (state != MenuState::OPTION) return;

	int BGM_vol = static_cast<int>((Game::BGM_volume / 255.0) * 100.0);
	int Sound_vol = static_cast<int>((Game::SE_volume / 255.0) * 100.0);

	std::string BGM_vol_str = std::to_string(BGM_vol);
	std::string Sound_vol_str = std::to_string(Sound_vol);

	TextureManager::render_text(BGM_vol_str +'%', Game::font2, very_light_gray, text_texture, 800, 360);
	TextureManager::render_text(Sound_vol_str + '%', Game::font2, very_light_gray, text_texture, 800, 480);

	TextureManager::render_from_texture((selectedOption_1 == 0) ? BGMSL : BGMUSL, BGM_X, BGM_Y, 3, 0, SDL_FLIP_NONE);
	TextureManager::render_from_texture((selectedOption_1 == 1) ? SoundSL : SoundUSL, SOUND_X, SOUND_Y, 3, 0, SDL_FLIP_NONE);
	TextureManager::render_from_texture((selectedOption_1 == 2) ? QuitSL : QuitUSL, QUIT_X_2, QUIT_Y_1, 2, 0, SDL_FLIP_NONE);
}

void Menu::render_title() { 
	if (state == MenuState::OPTION) return;
	static int title_x = TITLE_X_0;
	static int start_x = START_X_0;
	static int option_x = OPTION_X_0;
	static int quit_x = QUIT_X_0;

	if (state == MenuState::MAIN) {
		title_x -= 10;
		start_x -= 10;
		quit_x -= 10;
		option_x -= 10;
		if (title_x <= TITLE_X_1) title_x = TITLE_X_1;
		if (start_x <= START_X_1) start_x = START_X_1;
		if (option_x <= OPTION_X_1) option_x = OPTION_X_1;
		if (quit_x <= QUIT_X_1) quit_x = QUIT_X_1;
	}

	TextureManager::render_from_texture(title_texture, title_x, TITLE_Y, 2, 0, SDL_FLIP_NONE);

	TextureManager::render_from_texture((selectedOption == 0) ? StartSL : StartUSL, start_x, START_Y, 2, 0, SDL_FLIP_NONE);
	TextureManager::render_from_texture((selectedOption == 1) ? OptionSL : OptionUSL, option_x, OPTION_Y, 2, 0, SDL_FLIP_NONE);
	TextureManager::render_from_texture((selectedOption == 2) ? QuitSL : QuitUSL, quit_x, QUIT_Y, 2, 0, SDL_FLIP_NONE);
}