#include "headers/Game.hpp"
#include "headers/Bullets.hpp"
#include "headers/Player.hpp"
#include "headers/Enemy.hpp"
#include "headers/EnemyLayout.hpp"
#include "headers/TextureManager.hpp"
#include "headers/SoundManager.hpp"
#include <iostream>
#define endl "\n"

std::unordered_map<std::string, Mix_Chunk*> SoundManager::soundEffects; // Map to store sound effects
std::unordered_map<std::string, Mix_Music*> SoundManager::musicTracks; // Map to store music tracks

bool SoundManager::LoadSound(const std::string& id, const char* fileName) {
	Mix_Chunk* sound = Mix_LoadWAV(fileName); // Load sound effect from file
	if (!sound) {
		std::cout << "Failed to load sound: " << Mix_GetError << endl; // Print error if sound loading fails
		return false;
	}

	//for (std::pair<const std::string, Mix_Chunk*>& pair : soundEffects) {
	//	std::cout << pair.first << endl; // Debug: print loaded sound effect IDs
	//}

	soundEffects[id] = sound; // Add sound effect to the map
	return true;
}

void SoundManager::PlaySound(const std::string& id, int loop, int vol) {
	if (soundEffects.find(id) != soundEffects.end()) { // Check if sound effect exists in the map
		Mix_VolumeChunk(soundEffects[id], vol); // Set volume for the sound effect
		Mix_PlayChannel(-1, soundEffects[id], loop); // Play the sound effect
	}
	else {
		std::cout << "Sound " << id << " not found!" << endl; // Print error if sound effect is not found
	}
}

bool SoundManager::LoadMusic(const std::string& id, const char* fileName) {
	Mix_Music* music = Mix_LoadMUS(fileName); // Load music track from file
	if (!music) {
		std::cout << "Failed to load music: " << Mix_GetError << endl; // Print error if music loading fails
		return false;
	}

	//for (std::pair<const std::string, Mix_Music*>& pair : musicTracks) {
	//	std::cout << pair.first << endl; // Debug: print loaded music track IDs
	//}

	musicTracks[id] = music; // Add music track to the map
	return true;
}

void SoundManager::PlayMusic(const std::string& id, int loop, int vol) {
	if (musicTracks.find(id) != musicTracks.end()) { // Check if music track exists in the map
		Mix_VolumeMusic(vol); // Set volume for the music track
		Mix_PlayMusic(musicTracks[id], loop); // Play the music track
	}
	else {
		std::cout << "Music " << id << " not found!" << endl; // Print error if music track is not found
	}
}

void SoundManager::StopMusic() {
	Mix_HaltMusic(); // Stop playing music
}

void SoundManager::PauseMusic() {
	Mix_PauseMusic();
}

void SoundManager::ResumeMusic() {
	Mix_ResumeMusic();
}

void SoundManager::Clean() {
	for (std::pair<const std::string, Mix_Chunk*>& pair : soundEffects) { // Iterate through sound effects
		Mix_FreeChunk(pair.second); // Free the sound effect
	}
	soundEffects.clear(); // Clear the sound effects map

	for (std::pair<const std::string, Mix_Music*>& pair : musicTracks) { // Iterate through music tracks
		Mix_FreeMusic(pair.second); // Free the music track
	}
	musicTracks.clear(); // Clear the music tracks map
}
