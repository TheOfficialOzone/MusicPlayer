#pragma once


#include <iostream>

#include "SDL.h"
#include "Interactables/Interactables.h"


/*
* Handles all inputs made by the user
*/
namespace Input {
	//Updates the Mouse Controllers info
	void update(InteractableManager*);

	//Checks if the user wants to quit the program
	bool getExit();
};


/*
* Handles all actions performed by the mouse
*/
namespace Mouse {
	//Gets the mouse position
	void getMousePosition(int& x, int& y);

	//Get the mouses X position
	int getX();

	//Get the mouses Y position
	int getY();

	//Checks if the mouse is Down or not
	bool getLMBDown();
}
