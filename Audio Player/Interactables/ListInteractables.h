#pragma once


#include "Interactables.h"


#include <vector>

/*
* Contains other Interactables in a Container
*/
class ContainerInteractable : virtual public Interactable, virtual public InteractableManager {
private:
	//The texture all sub-interactables are rendered to
	SDL_Texture* renderTexture;

	//Initializes the Container Interactable
	void init();

	//Clears the Texture
	void clearTexture();

	//Generates the renderTexture
	int generateTexture();

	/// Binding Interactables

	//Binds Interactables to the Containers area
	int bindInteractablesToArea();

	/// Rendering

	//Renders the interactables onto the Container Interactable
	virtual int renderInteractables();
public:
	//Default Constructor
	ContainerInteractable();

	//Deconstructor
	~ContainerInteractable();

	//Adds an Interactable to the Container Interactable
	bool addInteractable(Interactable*);

	//Updates the Container Interactable
	int update();

	//Updates all the interactables
	int updateInteractables();

	/// Interactivity

	//Attemps to click the List Interactable
	int click(int, int);

	//On Mouse Down, will allow the sub-interactables to respond
	int mouseDown(int, int);

	//On Mouse Scroll, will allow the sub-interactables to respond
	int mouseScroll(int, int, float);


	//Generates an SDL_Rect to bind sub interactables to
	virtual SDL_Rect genBindingRect() const;

	/// Rendering

	//Renders the List Interactable
	void render();
};

/*
* The List Interactable is a container Interactable that is scrollable
*/
class ListInteractable : public ContainerInteractable {
private:
	//The distance the scrolled
	int scrollDistance;

	//The max distance the mouse may scroll
	int maxScrollDistance;

	//Initializes the List Interactable
	void init();
public:
	//Default Constructor
	ListInteractable();

	//Deconstructor
	~ListInteractable();

	/// Setters

	//Sets the scroll distance
	int setScrollDist(int);

	//Sets the max scroll distance
	int setMaxScrollDist(int);

	/// Getters

	//Generates an SDL_Rect to bind sub interactables to
	SDL_Rect genBindingRect() const;

	//Gets how far the mouse is currently scrolled
	int getScrollDist() const;

	//Gets the maximum scroll distance
	int getMaxScrollDist() const;

	/// Interactivity

	//What to do when the mouse is scrolled on the interactable
	int mouseScroll(int, int, float);
};