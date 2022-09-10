#pragma once



#include <future>
#include "Interactables/Interactables.h"
#include "Interactables/ListInteractables.h"
#include "Music/MusicPlayer/MusicPlayer.h"
#include "Music/MusicLoader/MusicLoader.h"
#include "Globals/Globals.h"

/*
* The interactable that allows the user to play / pause the music
*/
class PlayPauseInteractable : public TextureInteractable {
private:
	//Stores the texture for the play button
	SDL_Texture* playTexture = nullptr;

	//Is it currently paused
	bool paused;

	//Initializes the Interactable
	void init();
public:
	//Default Constructor
	PlayPauseInteractable();

	//Deconstructor
	~PlayPauseInteractable();

	//Updates the Volume Interactable
	int update();

	//Clears the textures from the interactable
	void clearTexture();

	/// Interactivity

	//Attemps to click the PlayPauseInteractable
	int click(int, int);

	/// Rendering

	//Renders the PlayPauseInteractable to the display
	void render();
};

/*
* Allows the user to adjust the musics volume
*/
class VolumeInteractable : public Interactable {
private:
	//The volume displayed, do not alter this variable other than in update!
	float volume;

	//Initializes the volume interactable
	void init();

	//Gets a volume from a click position
	float clickPositionToVolume(int, int);

	//Updates the volume from a click
	int updateVolumeFromClick(int, int);
public:
	//Default constructor
	VolumeInteractable();

	//Deconstructor
	~VolumeInteractable();

	//Updates the Volume Interactable
	int update();

	/// Interactivity

	//Attemps to click the Volume Interactable
	int click(int, int);

	//Attempts to interact with the volume Interactable when the mouse is down
	int mouseDown(int, int);

	//Changes the volume on a mouse scroll
	int mouseScroll(int, int, float);

	/// Rendering

	//Renders the Volume Interactable
	void render();
};

/*
* Allows for skipping forward a song
*/
class SkipForwardInteractable : public TextureInteractable {
private:
	//Initializes the Skip forward interactable
	void init();
public:
	//Default constructor
	SkipForwardInteractable();

	// Plays the next song on click
	int click(int, int);
};


/*
* Allows for backward a song
*/
class SkipBackwardInteractable : public TextureInteractable {
private:
	//Initializes the Skip forward interactable
	void init();
public:
	//Default constructor
	SkipBackwardInteractable();

	//Plays the previous song on click
	int click(int, int);
};


/*
* Adds music to the music list
*/
class MusicListInteractable : public ListInteractable {
private:
	//The Height to add the next Song at
	int addHeight;

	//Initializes the Music List
	void init();
public:
	//Default Constructor
	MusicListInteractable();

	//Deconstructor
	~MusicListInteractable();

	/// Adding to the list

	//Overides the parent function to disable directly adding Interactables
	bool addInteractable(Interactable*) override;

	//Adds music from the paths / Titles
	int addMusic(const std::vector<SongData>* songData);

	//Adds one song based off the path and title
	int addSong(const SongData data);

	//Renders the Music List
	void render();
};


/*
* Displays a single song
*/
class SongDisplayInteractable : public TextInteractable {
private:
	//Is this a valid song?
	bool validSong;

	//Is this being played
	bool beingPlayed;

	//Holds the songs data
	SongData songData;

	//Initializes the Song Display Interactable
	void init();
public:
	//Default Constructor
	SongDisplayInteractable();

	//Deconstructor
	~SongDisplayInteractable();

	//Called each frame
	//Checks if the song contained within is the same as the one that's playing
	int update() override;

	/// Setters

	//Sets the song to display
	int setSong(SongData);

	/// Getters

	//Checks if the Song Display has a valid song
	bool hasValidSong() const;

	//Checks if the Song Display is being played
	bool getBeingPlayed() const;

	/// Interactivity
	
	//Plays the saved song on click
	int click(int, int);

	/// Rendering

	//Renders the Song Display Interactable
	void render();
};
