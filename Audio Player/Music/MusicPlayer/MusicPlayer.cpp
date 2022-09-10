
#include <random>
#include "MusicPlayer.h"
#include "Globals/Globals.h"

#include <iostream>

#include "Music/MusicLoader/MusicLoader.h"

static float volume = 0.2;
static bool paused = false;	//Paused
static int songOn = 0;

//Gets the playing songs ID
static int currentSongID = -1;

static std::vector<Mix_Chunk*>* audio = nullptr;
static std::vector<Mix_Music*>* music = nullptr;
static std::vector<std::string>* previousSongPaths = nullptr;	//Stores all the previous paths


/*
* Loads the song from a file
* 
* @param file, The location of the song
* @return Mix_Chunk*, A "chunk" of the musics audio
*/
static Mix_Chunk* loadSong(std::string file) {
	return Mix_LoadWAV(file.c_str());
}

/*
* Loads the music from a file
*
* @param file, The location of the music
* @return Mix_Music*, The music from the file
*/
static Mix_Music* loadMusic(std::string file) {
	return Mix_LoadMUS(file.c_str());
}



/*
* Initializes the MusicPlayer
* 
* @return bool, true if the music player was initialized
*/
bool MusicPlayer::init() {
	SDL_assert(!loaded());
	//If it's already loaded we can skip
	if (loaded()) return true;

	bool initialized = true;

	// open 44.1KHz, signed 16bit, system byte order,
	// stereo audio, using 4096 byte chunks
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
		printf("Mix_OpenAudio: %s\n", Mix_GetError());
		exit(2);
		initialized = false;
	}

	audio = new std::vector<Mix_Chunk*>();
	music = new std::vector<Mix_Music*>();
	previousSongPaths = new std::vector<std::string>();

	return initialized;
}

/*
* Closes the Music Player
*/
void MusicPlayer::close() {
	haltMusic();
	Mix_CloseAudio();
}

/*
* 
* @return true, The music player is loaded
* @return false, The music player must be loaded
*/
bool MusicPlayer::loaded() {
	return audio != nullptr && music != nullptr && previousSongPaths != nullptr;
}


/*
* Sets the volume of the musicPlayer
* 
* @params newVolume, The volume to use
* @return True, the volume was changed
* @return False, the volume was not changed
*/
bool MusicPlayer::setVolumeLinear(float newVolume) {
	//Ensures the music player is loaded before attempting
	SDL_assert(loaded());
	if (!loaded()) return false;

	SDL_assert(0 <= newVolume && newVolume <= 1);
	bool changed = false;

	if (0 <= newVolume && newVolume <= 1) {
		volume = newVolume;
		changed = (Mix_VolumeMusic(MIX_MAX_VOLUME * volume) != -1);
	}
	return changed;
}

/*
* Gets the volume from the music player
* 
* @return float, The volume of the player from (0 - 1)
*/
float MusicPlayer::getVolume() { return volume; }

/*
* Gets the ID of the currently playing song
* 
* @return int, The song's ID, -1 on error
*/
int MusicPlayer::getPlayingSongID() { return currentSongID; }

/*
* Plays the next song and saves it to the buffer
* 
* @param file, The songs filepath
* @return true, The song was played
* @return false, The song was not played
*/
bool MusicPlayer::playSongSave(std::string file) {
	//Ensures the music player is loaded before attempting
	SDL_assert(loaded());
	if (!loaded()) return false;

	bool played = playSong(file);
	if (played)
		previousSongPaths->insert(previousSongPaths->begin(), file);	//Adds
	return played;
}

/*
* Plays the next song
* 
* @param file, The songs filepath
* @return true, The song was played
* @return false, The song was not played
*/
bool MusicPlayer::playSong(std::string file) {
	//Ensures the music player is loaded before attempting
	SDL_assert(loaded());
	if (!loaded()) return false;

	//Unpauses when playing new song
	paused = false;	

	//Loads the song
	Mix_Music* song = loadMusic(file);

	if (!song) {
		printf((file + " had an error loading %s \n").c_str(), Mix_GetError());
		return false;
	}

	Mix_HaltMusic();
	//Attempts to play the song
	if (Mix_PlayMusic(song, 0) == -1) {
		printf("Mix_PlayChannel: %s\n", Mix_GetError());
		return false;	//Returns 0 on failure
	}

	//Adds the audio to the list
	music->push_back(song);
	currentSongID = MusicLoader::getSongIDFromPath(file);

	return true;	//Returns 1 on success
}


/*
* Plays the song given from the songs ID, and saves it to the previous song buffer
*
* @param songID, the identifying ID of the song
* @return true, The song was played
* @return false, The song was not played
*/
bool MusicPlayer::playSongSave(int songID) {
	return playSongSave(MusicLoader::getMusicPathFromID(songID));
}

/*
* Plays the song given from the songs ID
* 
* @param songID, the identifying ID of the song
* @return true, The song was played
* @return false, The song was not played
*/
bool MusicPlayer::playSong(int songID) {
	return playSong(MusicLoader::getMusicPathFromID(songID));
}


/*
* Plays a random song
* 
* @return true, A random song was played
* @return false, Error playing song
*/
bool MusicPlayer::playRandomSong() {
	//Ensures the music player is loaded before attempting
	SDL_assert(loaded());
	if (!loaded()) return false;
	
	//Gets the music paths
	const std::vector<std::string>* musicPaths = MusicLoader::getMusicPaths();

	//Uniform randomness
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> distribution(0, musicPaths->size() - 1);
	
	//Plays the random song
	return playSongSave(musicPaths->at(distribution(gen)));	
}

/*
* Plays the previous song
* 
* @return true, The previous song was played
* @return false, No song was played
*/
bool MusicPlayer::playPreviousSong() {
	//Ensures the music player is loaded before attempting
	SDL_assert(loaded());
	if (!loaded()) return false;

	bool played = false;
	//Checks that the previous song exists
	if (songOn + 1 < (int)previousSongPaths->size()) {
		songOn++;
		played = playSong(previousSongPaths->at(songOn));
	}
	return played;
}

/*
* Plays the next song
* 
* @return true, The next song was played
* @return false, Error playing next song
*/
bool MusicPlayer::playNextSong() {
	//Ensures the music player is loaded before attempting
	SDL_assert(loaded());
	if (!loaded()) return false;

	bool played = false;
	//If it played a previous song
	if (songOn != 0) {	
		songOn--;
		//Skips the saving step
		played = playSong(previousSongPaths->at(songOn));
	} 
	//If the song is not from the buffer
	else {	
		played = playRandomSong();	
	}
	return played;
}


/*
* Skips to a position in the song, UNUSED
*/
void skipToPosition(double time) {
	SDL_assert(false);
	time = 1;
}

/*
* Toggles the music between play / pause
* 
* @return true, The music was toggled
* @return false, The music was not toggled
*/
bool MusicPlayer::toggleMusic() {
	//Ensures the music player is loaded before attempting
	SDL_assert(loaded());
	if (!loaded()) return false;

	//Toggles the music
	if (getPaused())
		resumeMusic();
	else
		pauseMusic();
	return true;
}

/*
* Checks if the music player getPaused
* 
* @return true, The music is paused
* @return false, The music is playing
*/
bool MusicPlayer::getPaused() { 
	return paused; 
}


/*
* Resumes the music
*/
void MusicPlayer::resumeMusic() {
	//Ensures the music player is loaded before attempting
	SDL_assert(loaded());

	paused = false;
	Mix_ResumeMusic();
}

/*
* Pauses the music
*/
void MusicPlayer::pauseMusic() {
	//Ensures the music player is loaded before attempting
	SDL_assert(loaded());

	paused = true;
	Mix_PauseMusic();
}

/*
* Halts the players music
*/
void MusicPlayer::haltMusic() {
	Mix_HaltMusic();
}

/*
* Deletes the music track
*/
void MusicPlayer::deleteMusicTrack() {
	//Ensures the music player is loaded before attempting
	SDL_assert(loaded());

	//Defaults the current songs ID to -1
	currentSongID = -1;

	//Deletes the music
	Mix_Music* song = music->at(0);
	//Removes it from the list
	music->erase(music->begin());
	//Frees the song!
	Mix_FreeMusic(song);
}
