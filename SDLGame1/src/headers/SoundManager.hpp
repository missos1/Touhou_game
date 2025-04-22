#ifndef SOUND_MANAGER_HPP
#define SOUND_MANAGER_HPP

#include <SDL.h>
#include <SDL_mixer.h>
#include <string>
#include <unordered_map>
#include <set>

class Game;
class Bullet;
class Player;
class Enemy;
class TextureManager;

class SoundManager {
public:
	static bool LoadSound(const std::string& id, const char* fileName);
	static void PlaySound(const std::string& id, int loop, int vol);
	static bool LoadMusic(const std::string& id, const char* fileName);
	static void PlayMusic(const std::string& id, int loop, int vol);
	static void SetVolumeMusic(int vol);
	static void StopMusic();
	static void PauseMusic();
	static void ResumeMusic();
	static void Clean();

private:
	static std::unordered_map<std::string, Mix_Chunk*> soundEffects;
	static std::unordered_map<std::string, Mix_Music*> musicTracks;
};

#endif