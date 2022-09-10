


#include "MusicLoader.h"

#include <iostream>	//IO
#include <fstream>
#include <filesystem>

#include <algorithm>

#include <memory>	//Smart Pointers
#include "SDL.h"	//SDL
#include "SDL_mixer.h"	//Music

#include <SDL_assert.h>


//Default the ID generator to 0
int SongData::IDgenerator = 0;

//Used to store the music loader data
static std::vector<std::string>* songTitles = nullptr;
static std::vector<std::string>* songPaths = nullptr;
static std::vector<SongData>* songDatas = nullptr;

/*
* Default constructor
*/
SongData::SongData() {
	title = "";
	path = "";
	ID = -1;
}

/*
* Generates Song Data for a title and path
* 
* @param title, The title of the song
* @param path, The path of the song
*/
SongData::SongData(std::string title, std::string path) 
	: title(title), path(path), ID(IDgenerator++) {}

/*
* Deconstructor
* Nothing to delete atm
*/
SongData::~SongData() {}


/*
* The assignment operator
*/
SongData& SongData::operator= (const SongData& other) {
	//Copies all the data from the song data
	title = other.getTitle();
	path = other.getPath();
	ID = other.getID();

	return *this;
}

/*
* Gets the title from the Song
* 
* @return string, The title
*/
std::string SongData::getTitle() const { return title; }

/*
* Gets the path from the Song
*
* @return string, The path
*/
std::string SongData::getPath() const { return path; }


/*
* Gets the ID from the Song
*
* @return int, the ID, -1 if invalid
*/
int SongData::getID() const { return ID; }

/*
* Checks if the SongData is valid
* 
* @return true, The song data is valid
* @return false, The song data is invalid
*/
bool SongData::getValid() const { return ID != -1; }

/*
* Initializes the musicLoader
* 
* @return bool, Always true (the music is initialized)
*/
bool MusicLoader::init() {
	//If it's already loaded it will return automatically
	SDL_assert(!loaded());
	if (loaded()) return true;

	//Creates the list for the titles and paths
	songTitles = new std::vector<std::string>;
	songPaths = new std::vector<std::string>;
	songDatas = new std::vector<SongData>;

	return true;
}

/*
* Closes the musicLoader
*/
void MusicLoader::close() {
	SDL_assert(loaded());

	//Checks that the MusicLoader is loaded
	if (loaded()) {
		//Deletes the stored data
		songTitles->clear();
		songPaths->clear();
		songDatas->clear();

		delete songTitles;
		delete songPaths;
		delete songDatas;

		songTitles = nullptr;
		songPaths = nullptr;
		songDatas = nullptr;
	}
}

/*
* Checks if the Musicloader is loaded
* 
* @return bool, true if the musicloader is loaded
*/
bool MusicLoader::loaded() {
	return songPaths != nullptr && songTitles != nullptr;
}

/*
* Converts all backslashes to slashes (So the rest of the code doesn't have to check both)
*
* @params std::vector<std::string>* songs, The vector of strings to look through
* @return bool, True if the backslashes were removed
*/
bool removeBackslashs(std::vector<std::string>* songs) {
	SDL_assert(songs != nullptr);
	bool backSlashesRemoved = false;

	if (songs != nullptr) {
		//The Backslashes will be removed properly
		backSlashesRemoved = true;

		//Loops through every character in every word changing all '\\' to '/' for consistency
		for (int i = 0; i < songs->size(); i++) {
			std::string* path = &songs->at(i);
			std::replace(path->begin(), path->end(), '\\', '/');
		}
	}

	return backSlashesRemoved;
}

/*
* Removes anything that isn't an mp3 file
*
* @params std::vector<std::string>* songs, The vector of strings to look through
* @return bool, True if the vector only contains .mp3 files
*/
bool removeNonMusic(std::vector<std::string>* songs) {
	//Ensures that the parameters are valid
	SDL_assert(songs != nullptr);
	if (songs == nullptr) return false;

	//Loops through each path in the vector
	for (int i = 0; i < songs->size(); i++) {
		const std::string path = songs->at(i);

		//Checks that it has the .mp3 file extension
		if (path.substr(path.find_last_of(".") + 1) != "mp3") {
			//Removes it from the list if it doesn't
			std::cout << songs->at(i) << " removed not a music file!" << std::endl;
			songs->erase(songs->begin() + i);
			i--; //Rolls back 1 since it just deleted 1 element
		}
	}
	
	return true;
}


/*
* Generates Music titles from a list of songs
*
* @params std::vector<std::string>* songs, The vector of strings to look through
* @return bool, True if the vector only contains .mp3 files
*/
bool generateMusicTitles(std::vector<std::string>* songs) {
	//Ensures the parameters are valid
	SDL_assert(songs != nullptr);
	if (songs == nullptr) return false;

	//Loops through each song path
	for (int i = 0; i < songPaths->size(); i++) {
		const std::string path = songs->at(i);
		std::string title = "";

		//Gets the substring between the last '/' and '.'
		size_t afterSlash = path.find_last_of('/') + 1;
		size_t beforePeriod = path.find_last_of('.');

		title = "";
		//Ensures the substring is valid
		if (beforePeriod > afterSlash)
			title = path.substr(afterSlash, beforePeriod - afterSlash);

		//Adds the title
		songTitles->push_back(title);

		//Adds new song Data
		songDatas->push_back(SongData(title, path));
	}
	
	return true;
}


/*
* Loads music from a folder
* 
* @params musicPath The folder where the music is loaded, 
* @return int, The amount of songs in the folder, -1 if there was an error
*/
int MusicLoader::getMusicListFromFolder(std::string musicPath) {
	//Ensures the MusicLoader is loaded
	SDL_assert(loaded());
	if (!loaded()) return -1;

	namespace fs = std::filesystem;

	//Loads all the files in the directory to this vector
	for (auto& p : fs::directory_iterator(musicPath)) {
		songPaths->push_back(p.path().u8string());
	}

	//Removes all elements that aren't music
	bool validSongs = removeNonMusic(songPaths) && removeBackslashs(songPaths);
	
	//Generates titles for all the music
	validSongs = generateMusicTitles(songPaths) && validSongs;
	SDL_assert(validSongs);

	//Returns the amount of songs
	return (validSongs ? songPaths->size() : -1);
}



/*
* Gets the musics Path from it's ID
* 
* @param ID, The Identifying ID of the songData
* @return string, The path to the song
*/
std::string MusicLoader::getMusicPathFromID(int ID) {
	return songDatas->at(ID).getPath();
}

/*
* Gets the musics Title from it's ID
*
* @param ID, The Identifying ID of the songData
* @return string, The Title to the song
*/
std::string MusicLoader::getMusicTitleFromID(int ID) {
	return songDatas->at(ID).getTitle();
}



/*
* Gets the Song ID from the path
*
* @param path, The path to the song
* @return int, The song's ID, -1 on error
*/
int MusicLoader::getSongIDFromPath(std::string path) {
	//Ensures the Music loader is... loaded
	SDL_assert(loaded());
	if (!loaded()) return -1;

	int ID = -1;

	//Loops until it finds the songs ID
	for (SongData sd : *songDatas) {
		if (path == sd.getPath())
			ID = sd.getID();
	}

	return ID;
}


/*
* Gets the music titles 
*/
const std::vector<std::string>* MusicLoader::getMusicTitles() { 
	SDL_assert(loaded());
	return songTitles; 
}

/*
* Gets the music paths
*/
const std::vector<std::string>* MusicLoader::getMusicPaths() { 
	SDL_assert(loaded());
	return songPaths; 
}

/*
* Gets the song data
*/
const std::vector<SongData>* MusicLoader::getSongData() {
	SDL_assert(loaded());
	return songDatas;
}


/*
* Gets the music titles Mutably
*/
std::vector<std::string>* MusicLoader::getMusicTitlesMut() {
	SDL_assert(loaded());
	return songTitles; 
}

/*
* Gets the music paths Mutably
*/
std::vector<std::string>* MusicLoader::getMusicPathsMut() {
	SDL_assert(loaded());
	return songPaths; 
}

/*
* Gets the Song Data Mutably
*/
std::vector<SongData>* MusicLoader::getSongDataMut() {
	SDL_assert(loaded());
	return songDatas;
}
