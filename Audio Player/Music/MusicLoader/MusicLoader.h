#pragma once

#include <string>
#include <vector>


/*
* Stores the Songs data
* A songs data is its...
*	Title
*	Path
*	ID
*/
class SongData {
private:
	static int IDgenerator;
	//Title of the song
	std::string title;
	//Path to the song
	std::string path;
	//ID of the song
	int ID;

public:
	//Default Constructor
	SongData();

	//Null Constructor
	SongData(std::string title, std::string path);

	//Deconstructor
	~SongData();

	//The assignment operator
	SongData& operator= (const SongData& other);

	//Gets the title of the song
	std::string getTitle() const;

	//Gets the path to the song
	std::string getPath() const;

	//Gets the ID of the song
	int getID() const;

	//Checks if the SongData is valid
	bool getValid() const;

};

/*
* This namespace is used to load the music from the file system
*/
namespace MusicLoader {
	//Initializes the musicLoader
	bool init();

	//Closes the MusicLoader
	void close();

	//Checks if the MusicLoader is initialized
	bool loaded();

	//Gets a music list from a folder
	int getMusicListFromFolder(std::string musicPath);


	//Gets the musics Path from it's ID
	std::string getMusicPathFromID(int);

	//Gets the musics Title from it's ID
	std::string getMusicTitleFromID(int);


	//Gets the Song ID from the path
	int getSongIDFromPath(std::string);

	//Gets the Music Titles / Paths as constants
	const std::vector<std::string>* getMusicTitles();
	const std::vector<std::string>* getMusicPaths();
	const std::vector<SongData>* getSongData();

	//Gets the Music Titles / Paths mutably
	std::vector<std::string>* getMusicTitlesMut();
	std::vector<std::string>* getMusicPathsMut();
	std::vector<SongData>* getSongDataMut();
};
