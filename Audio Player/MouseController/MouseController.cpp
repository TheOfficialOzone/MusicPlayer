


#include "MouseController.h"
#include "Music/MusicPlayer/MusicPlayer.h"
#include "Globals/Globals.h"



//Should the application quit
static bool quit = false;

/*
* ONLY run when mouse is pressed
* Makes all interactive elements respond to the current mouse click
*/
void onMousePressed() {
	//Doesn't do anything rn
}

/*
* ONLY run when mouse is released
* Currently unused
*/
void onMouseReleased(InteractableManager* manager) {
	//Ensures the manager is valid
	SDL_assert(manager != nullptr);
	if (manager == nullptr) return;
	
	//Clicks on the manager
	manager->click(Mouse::getX(), Mouse::getY());
}


/*
* ONLY run when mouse is down
*/
void onMouseDown(InteractableManager* manager) {
	//Ensures the manager is valid
	SDL_assert(manager != nullptr);
	if (manager == nullptr) return;

	//Checks if it clicks any interactables
	manager->mouseDown(Mouse::getX(), Mouse::getY());
}

/*
* Runs when the mouse wheel is scrolled
*/
void onMouseScroll(InteractableManager* manager, float scrollSpd) {
	manager->mouseScroll(Mouse::getX(), Mouse::getY(), scrollSpd * 10);
	//InteractableManager::checkScroll(Mouse::getX(), Mouse::getY(), scrollSpd);	//Checks the scroll of the Interactable managers
}



/*
* Recieves inputs for the mouse controller
* 
* @return int, 0 on success
*/
int recieve(InteractableManager* manager) {
	SDL_Event event;
	//Loops through each event
	while (SDL_PollEvent(&event)) {
		//Gets the keypresses
		const Uint8* state = SDL_GetKeyboardState(NULL);

		//If the mouse is scrolled
		if (event.type == SDL_MOUSEWHEEL) {
			onMouseScroll(manager, (float)event.wheel.y);
		}

		//Quits the program
		if (event.type == SDL_QUIT)
			quit = true;


		/// Multimedia keys
		if (state[SDL_SCANCODE_AUDIONEXT]) {
			MusicPlayer::playNextSong();
		}
		if (state[SDL_SCANCODE_AUDIOPLAY]) {
			MusicPlayer::toggleMusic();
		}
		if (state[SDL_SCANCODE_AUDIOPREV]) {
			MusicPlayer::playPreviousSong();
		}
		if (state[SDL_SCANCODE_AUDIOSTOP]) {
			MusicPlayer::pauseMusic();
		}
	}

	return 0;	//returns 0 when done
}


/*
* Updates the mouse Controllers info
*/
void Input::update(InteractableManager* manager) {
	// Ensures the manager is valid
	SDL_assert(manager != nullptr);
	if (manager == nullptr) return;

	//Pumps the event loop
	SDL_PumpEvents();

	//Gets the status of the LMB
	static bool LMBDown;
	bool LMBIsDown = Mouse::getLMBDown();

	//If the left mouse is down it clicks
	if (LMBIsDown)
		onMouseDown(manager);

	//If they do not match! (It was either released or pressed)
	if (LMBIsDown != LMBDown)
		if (LMBIsDown) 	
			//Mouse just got Pressed
			onMousePressed();
		else 	
			//Mouse just got released
			onMouseReleased(manager);

	//Updates whether or not the LMB is down
	LMBDown = LMBIsDown;
	//Recieves the inputs
	recieve(manager);
}

/*
* Checks if the user wants to quit the program
* 
* @return true, User wants to quit
* @return false, Continue running the program
*/
bool Input::getExit() {
	return quit;
}

/*
* Gets the mouse position
* 
* @param x, Will be filled with the mouse's X position
* @param y, Will be filled with the mouse's Y position
*/
void Mouse::getMousePosition(int& x, int& y) {
	SDL_GetMouseState(&x, &y);
}


/*
* Gets the mouse's X position
*
* @return int, the mouse's X position
*/
int Mouse::getX() {
	int x, y;
	getMousePosition(x, y);
	return x;
};

/*
* Gets the mouse's Y position
* 
* @return int, the mouse's Y position
*/
int Mouse::getY() {
	int x, y;
	getMousePosition(x, y);
	return y;
}

/*
* Checks if the left mouse button is down
* 
* @return bool, True -> The LMB is down
*/
bool Mouse::getLMBDown() {
	if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT))
		return true;
	return false;
}