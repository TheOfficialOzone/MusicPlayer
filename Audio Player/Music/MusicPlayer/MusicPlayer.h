#pragma once



#include "SDL.h"
#include "SDL_mixer.h"

#include "MusicPlayer.h"


/*
* Plays music given, and stores previously played music
*/
namespace MusicPlayer {
	//Allows for audio playback
	bool init();

	//Closes audio
	void close();

	//Checks if the musicPlayer is loaded
	bool loaded();

	//Plays the song and also saves to the previous song buffer
	bool playSongSave(std::string file);

	//Plays the song given, does not save the song, returns 0 on failure
	bool playSong(std::string file);

	//Plays the song given from the ID, and saves it to the previous song buffer
	bool playSongSave(int songID);

	//Plays the song given, from the songs ID
	bool playSong(int songID);

	//Plays a random song
	bool playRandomSong();

	//Plays the previous song
	bool playPreviousSong();

	//Plays the next song
	bool playNextSong();

	//Plays the song from a position
	void skipToPosition(double);

	/// Setters

	//Toggles the music between play / pause
	bool toggleMusic();

	//Sets the volume
	bool setVolumeLinear(float);

	//Resumes the music
	void resumeMusic();

	//Pauses the music
	void pauseMusic();

	//Halts the music completely
	void haltMusic();

	/// Getters 

	//Checks if it's paused
	bool getPaused();

	//Gets the volume
	float getVolume();

	//Gets the ID of the currently playing song
	int getPlayingSongID();

	//Deletes the most recent music trick
	void deleteMusicTrack();
};
