#include "headers/Game.hpp"
#include "headers/Bullets.hpp"
#include "headers/Player.hpp"
#include "headers/Enemy.hpp"
#include "headers/EnemyLayout.hpp"
#include "headers/TextureManager.hpp"
#include "headers/SoundManager.hpp"
#include <iostream>
#define endl "\n"

std::unordered_map<std::string, Mix_Chunk*> SoundManager::soundEffects;
std::unordered_map<std::string, Mix_Music*> SoundManager::musicTracks;

bool SoundManager::LoadSound(const std::string& id, const char* fileName) {
	Mix_Chunk* sound = Mix_LoadWAV(fileName);
	if (!sound) {
		std::cout << "Failed to load sound: " << Mix_GetError << endl;
		return false;
	}

	for (std::pair<const std::string, Mix_Chunk*>& pair : soundEffects) {
		std::cout << pair.first << endl;
	}

	soundEffects[id] = sound;
	return true;
}

void SoundManager::PlaySound(const std::string& id, int loop, int vol) {
	if (soundEffects.find(id) != soundEffects.end()) {
		Mix_VolumeChunk(soundEffects[id], vol);
		Mix_PlayChannel(-1, soundEffects[id], loop);
	}
	else {
		std::cout << "Sound " << id << " not found!" << endl;
	}
}

bool SoundManager::LoadMusic(const std::string& id, const char* fileName) {
	Mix_Music* music = Mix_LoadMUS(fileName);
	if (!music) {
		std::cout << "Failed to load music: " << Mix_GetError << endl;
		return false;
	}

	for (std::pair<const std::string, Mix_Music*>& pair : musicTracks) {
		std::cout << pair.first << endl;
	}

	musicTracks[id] = music;
	return true;
}

void SoundManager::PlayMusic(const std::string& id, int loop, int vol) {
	if (musicTracks.find(id) != musicTracks.end()) {
		Mix_VolumeMusic(vol);
		Mix_PlayMusic(musicTracks[id], loop);
	}
	else {
		std::cout << "Music " << id << " not found!" << endl;
	}
}

void SoundManager::StopMusic() {
	Mix_HaltMusic();
}

void SoundManager::Clean() {
	for (std::pair<const std::string, Mix_Chunk*>& pair : soundEffects) {
		Mix_FreeChunk(pair.second);
	}
	soundEffects.clear();

	for (std::pair<const std::string, Mix_Music*>& pair : musicTracks) {
		Mix_FreeMusic(pair.second);
	}
	musicTracks.clear();
}