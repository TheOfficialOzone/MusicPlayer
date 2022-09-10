


#include "MusicDisplayer.h"
#include "Music/MusicPlayer/MusicPlayer.h"

#include "SDL_assert.h"
#include "SDL_image.h"
#include "Globals/Globals.h"
#include "Globals/Display.h"
#include "Music/MusicPlayer/MusicPlayer.h"

#include <iostream>


//Initializes the Interactable
void PlayPauseInteractable::init() {
	setTexture("Images/pauseButton.png");
	setRenderStyle(RenderStyle::Centered);
	playTexture = loadTextureFromFile("Images/playButton.png");
}

/*
* Default constructor for the play pause interactable
*/
PlayPauseInteractable::PlayPauseInteractable() : TextureInteractable() {
	init();
}

/*
* Deconstructor for the playPause Interactable
*/
PlayPauseInteractable::~PlayPauseInteractable() {
	clearTexture();
}

/*
* Updates the volume Interactable
* 
* @return int, 0 on success, otherwise an error
*/
int PlayPauseInteractable::update() {
	int worked = TextureInteractable::update();

	//If the states are different
	if (paused != MusicPlayer::getPaused()) {
		//Switches the states and invalidates the Interactable
		paused = !paused;
		invalidate();
	}

	return worked;
}

/*
* Clears the textures from the interactable
*/
void PlayPauseInteractable::clearTexture() {
	//Clears the texture from the texture interactable
	TextureInteractable::clearTexture();

	//Deletes the current playTexture
	if (playTexture != nullptr) {
		SDL_DestroyTexture(playTexture);
		playTexture = nullptr;
	}
}

/*
* Attemps to click the PlayPauseInteractable
*
* @param clickX, The click's X position
* @param clickY, The click's Y position
* @return int, 0 on success otherwise an error occured
*/
int PlayPauseInteractable::click(int clickX, int clickY) {
	//Checks if the click overlaps with the interactable
	if (getPositionOverlap(clickX, clickY)) {
		//Toggles the music
		MusicPlayer::toggleMusic();
		//Forces an update
		invalidate();
	}

	return 0;
}


/*
* Renders the PlayPauseInteractable to the display
*/
void PlayPauseInteractable::render() {

	//Sets the color
	SDL_Color color = getPrimaryColor();
	SDL_SetRenderDrawColor(Display::getRenderer(), color.r, color.g, color.b, color.a);

	//Display the pause texture / play texture depending on the situation
	if (!paused) {
		TextureInteractable::render();
	} else {
		//Gets the rendered area
		SDL_Rect renderArea = getRect();
		//Copies the texture to the display
		SDL_RenderCopy(Display::getRenderer(), playTexture, NULL, &renderArea);
	}
	//Revalidates the Interactable as it has been rendered
	revalidate();
}



/*
* Initializes the Volume Interactable
*/
void VolumeInteractable::init() {
	//Sets the colors
	setPrimaryColor(255, 255, 255, 255);
	setSecondaryColor(0, 0, 0, 255);


	//Sets the volume
	volume = MusicPlayer::getVolume();
}

/*
* Default Constructor
*/
VolumeInteractable::VolumeInteractable() {
	init();
}


/*
* Deconstructor, Nothing to delete
*/
VolumeInteractable::~VolumeInteractable() {}


/*
* Gets a volume from a click position
*
* @param clickX, The click's X position
* @param clickY, The click's Y position
* @return float, 0 - 1 for the volume, -1 on error (not overlapping the button)
*/
float VolumeInteractable::clickPositionToVolume(int clickX, int clickY) {
	//Checks that the click overlaps
	if (getPositionOverlap(clickX, clickY)) {
		//Gets the percentage the click is from the left to right edge
		return ((float)clickX - getX()) / getW();
	}
	//-1 on error
	return -1;
}

/*
* Updates the volume from a click
*
* @param clickX, The click's X position
* @param clickY, The click's Y position
* @return int, 0 on success otherwise an error occured
*/
int VolumeInteractable::updateVolumeFromClick(int clickX, int clickY) {
	//Gets the new proposed value
	float newVolume = clickPositionToVolume(clickX, clickY);

	//Ensures the volume is valid
	if (newVolume == -1) return 1;
	
	//Updates the volume
	bool volumeChanged = MusicPlayer::setVolumeLinear(newVolume);
	invalidate();

	return !volumeChanged;
}

/*
* Changes the volume off a mouse click
*
* @param clickX, The click's X position
* @param clickY, The click's Y position
* @return int, 0 on success, otherwise an error occured
*/
int VolumeInteractable::click(int clickX, int clickY) {
	int updated = 0;
	//Updates the volume based off the click position
	if (getPositionOverlap(clickX, clickY)) {
		updated = updateVolumeFromClick(clickX, clickY);
	}

	return updated;
}

/*
* Changes the volume off a mouse down
* 
* @param downX, The Mouse Down's X position
* @param downY, The Mouse Down's Y position
* @return int, 0 on success otherwise an error occured
* */
int VolumeInteractable::mouseDown(int downX, int downY) {
	//Just calls the click
	return click(downX, downY);
}

/*
* Changes the volume off a mouse scroll
* 
* @param scrollX, The Mouse Scroll's X position
* @param scrollY, The Mouse Scroll's Y position
* @param scrollSpd, The Mouse Scroll's Speed
* @return int, 0 on success, otherwise an error occured
*/
int VolumeInteractable::mouseScroll(int scrollX, int scrollY, float scrollSpd) {
	//If there is no overlap immediately returns
	if (!getPositionOverlap(scrollX, scrollY)) return 0;
	
	float newVolume = volume + 0.01 * scrollSpd;

	newVolume = (newVolume > 1 ? newVolume = 1 : newVolume);
	newVolume = (newVolume < 0 ? newVolume = 0 : newVolume);
	//Sets the volume to the new volume
	if (MusicPlayer::setVolumeLinear(newVolume)) {
		volume = newVolume;
		invalidate();
		return 0;
	}
	//An error occured if the volume didn't change
	return 1;
}

/*
* Updates the Volume Interactable
* 
* @return int, 0 on success, otherwise an error occured
*/
int VolumeInteractable::update() {
	//Updates the interactable
	Interactable::update();

	//Updates the volume
	volume = MusicPlayer::getVolume();

	return 0;
}

/*
* Renders the Volume Interactable
* 
* @param renderer, The renderer to use
*/
void VolumeInteractable::render() {
	//Gets the color
	SDL_Color primaryColor = getPrimaryColor();
	SDL_Color secondaryColor = getSecondaryColor();
	
	//Gets the rect
	SDL_Rect renderArea = getRect();
	SDL_Rect lineArea = renderArea;

	//Thie is where the line will end
	lineArea.w = getW() * volume;

	//Renders the outline
	SDL_SetRenderDrawColor(Display::getRenderer(), secondaryColor.r, secondaryColor.g, secondaryColor.b, secondaryColor.a);
	SDL_RenderFillRect(Display::getRenderer(), &renderArea);

	//Renders the bar
	SDL_SetRenderDrawColor(Display::getRenderer(), primaryColor.r, primaryColor.g, primaryColor.b, primaryColor.a);
	SDL_RenderFillRect(Display::getRenderer(), &lineArea);

	//Revalidates the Interactable as it has been rendered
	revalidate();
}


/*
* Initializes the skip forward interactable*
*/
void SkipForwardInteractable::init() {
	setTexture("Images/skipButton.png");
	setRenderStyle(RenderStyle::Centered);
}

/*
* Default constructor for the Skip forward interactable
*/
SkipForwardInteractable::SkipForwardInteractable() : TextureInteractable() {
	init();
}

/*
* Attemps to click the Skip Forward Interactable
* Plays the next Song
* 
* @param clickX, The click's X position
* @param clickY, The click's Y position
* @return int, 0 on success, otherwise an error occured
*/
int SkipForwardInteractable::click(int clickX, int clickY) {
	bool played = false;
	//If the click overlaps play the next song
	if (getPositionOverlap(clickX, clickY)) {
		played = MusicPlayer::playNextSong();
	}

	return (played != 0);
}


/*
* Initializes the skip forward interactable
*/
void SkipBackwardInteractable::init() {
	setTexture("Images/backButton.png");
	setRenderStyle(RenderStyle::Centered);
}

/*
* Default constructor for the Skip forward interactable
*/
SkipBackwardInteractable::SkipBackwardInteractable() : TextureInteractable() {
	init();
}

/*
* Attemps to click the Skip Forward Interactable
* Plays the next Song
*
* @param clickX, The click's X position
* @param clickY, The click's Y position
* @return int, 0 on success, otherwise an error occured
*/
int SkipBackwardInteractable::click(int clickX, int clickY) {
	bool played = false;
	//If the click overlaps play the next song
	if (getPositionOverlap(clickX, clickY)) {
		played = MusicPlayer::playPreviousSong();
	}

	return (played != 0);
}



/*
* Initializes the Music List
*/
void MusicListInteractable::init() {
	setPrimaryColor(50, 50, 50, 255);
	//Defaults the height to 0
	addHeight = 0;
}

/*
* Default constructor for the Music List Interactable
*/
MusicListInteractable::MusicListInteractable() : ListInteractable() {
	init();
}

/*
* Deconstructor for the Music List Interactable
*/
MusicListInteractable::~MusicListInteractable() {}

/*
* Overides the parent function to disable directly adding Interactables
* 
* @param interactable, The Interactable to add
* @return false, Always false NEVER TRUE as this shouldn't be called
*/
bool MusicListInteractable::addInteractable(Interactable* interactable) { 
	SDL_assert(false);
	return false; 
}


/*
* Adds music from the paths / titles
* 
* @param paths, The music Paths
* @param titles, The music 
* @return int, The number of songs added, Any negative number means there was an error
*/

int MusicListInteractable::addMusic(const std::vector<SongData>* songData) {
	//Ensures valid parameters
	SDL_assert(songData != nullptr);
	if (songData == nullptr) return -1;

	//Adds all the songs
	for (int i = 0; i < songData->size(); i++) {
		addSong(songData->at(i));
	}

	return 1;
}

/*
* Adds a song to the Music List
* 
* @param path, The song path
* @param title, The song title
* @return int, 0 on success, Otherwise there was an error
*/
int MusicListInteractable::addSong(const SongData data) {
	//Creates the song
	SongDisplayInteractable* newSong = new SongDisplayInteractable();

	//Sets its position
	newSong->setX(CordType::Percentage, 0);
	newSong->setY(CordType::Pixel, addHeight);
	newSong->setW(CordType::PercentageWidth, 1);
	newSong->setH(CordType::Pixel, 75);

	//Binds the song to the area
	newSong->bindToArea(genBindingRect());

	//Sets the song
	newSong->setSong(data);

	//Adds the Interactable to the list
	ListInteractable::addInteractable(newSong);

	//The new scroll Distance is the addHeight of the song we just added
	setMaxScrollDist(addHeight);
	addHeight += newSong->getH();

	return 0;
}

/*
* Renders the Music List
*/
void MusicListInteractable::render() {
	ListInteractable::render();
}

/*
* Initializes the Song Display Interactable
*/
void SongDisplayInteractable::init() {
	//Sets the default color
	setPrimaryColor(0, 0, 0, 0);
	setTextColor(255, 255, 255, 255);
	//Defaults to an invalid song
	validSong = false;
	beingPlayed = false;
}

/*
* Default Constructor
*/
SongDisplayInteractable::SongDisplayInteractable() : TextInteractable() {
	init();
}

/*
* Deconstructor
*/
SongDisplayInteractable::~SongDisplayInteractable() {}

/*
* Called each frame
* Checks if the song contained within is the same as the one that's playing
* 
* @return int, 0 on success, otherwise an error occured
*/
int SongDisplayInteractable::update() {
	int textUpdated = TextInteractable::update();

	int playingSongID = MusicPlayer::getPlayingSongID();

	//If the song's being played, re-render
	bool playing = songData.getID() == playingSongID;

	//If the song's playing state switched
	if (getBeingPlayed() != playing) {
		beingPlayed = playing;

		//If it's being played it should be red, otherwise clear
		if (getBeingPlayed())
			setPrimaryColor(255, 50, 50, 255);
		else
			setPrimaryColor(0, 0, 0, 0);
		//Re-render this interactable
		invalidate();
	}

	return textUpdated;
}


/*
* Sets the song to display
*
* @param song, The song's data
* @return int, 0 on success, otherwise an error occured
*/
int SongDisplayInteractable::setSong(SongData song) {
	//Sets the songData
	songData = song;
	//Checks that the text changed
	bool worked = TextInteractable::setText(songData.getTitle());
	//Ensures that it is a valid song
	validSong = true;

	return worked;
}

/*
* Checks if the Song Display has a valid song
* 
* @return true, The Song Display contains a valid song
* @return false, The Song Display contains a invalid song
*/
bool SongDisplayInteractable::hasValidSong() const { return validSong; }


/*
* Checks if the Song Display is being played
*
* @return true, The Song Display is playing
* @return false, The Song Display is not playing
*/
bool SongDisplayInteractable::getBeingPlayed() const { return beingPlayed; }

/*
* Clicks on the Song Display
* 
* @param clickX, The click's X position
* @param clickY, The click's Y position
* @return int, 0 on success, otherwise an error occured
*/
int SongDisplayInteractable::click(int clickX, int clickY) {
	//Ensures that the it actually has a valid song
	if (!hasValidSong()) return 0;

	//Checks that the position overlaps
	if (getPositionOverlap(clickX, clickY)) {
		MusicPlayer::playSongSave(songData.getID());
	}
}


/*
* Renders the Song Display Interactable
*/
void SongDisplayInteractable::render() {
	Interactable::render();
	TextInteractable::render();
}