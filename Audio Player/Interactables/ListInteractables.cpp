#include "ListInteractables.h"

#include "Globals/Display.h"

#include <iostream>


/*
* Initializes the Container Interactable
*/
void ContainerInteractable::init() {
	renderTexture = nullptr;
}

/*
* Clears the Container Interactable's texture
*/
void ContainerInteractable::clearTexture() {
	//Delete the texture
	if (renderTexture != nullptr) {
		SDL_DestroyTexture(renderTexture);
		renderTexture = nullptr;
	}
}

/*
* Generates the Container Interactables texture
* 
* @return int, 0 on success, otherwise an error
*/
int ContainerInteractable::generateTexture() {
	//Clears the old texture
	clearTexture();

	//Creates the new texture
	renderTexture = SDL_CreateTexture(
		Display::getRenderer(), 
		SDL_PIXELFORMAT_ARGB8888, 
		SDL_TEXTUREACCESS_TARGET, 
		getW(), 
		getH()
	);

	//Returns 0 if the renderTexture is valid
	return (renderTexture != nullptr ? 0 : 1);
}

/*
* Binds the Interactables to the Containers Area
* 
* @return int, 0 on success, otherwise an error occured
*/
int ContainerInteractable::bindInteractablesToArea() {
	int binded = 0;
	//Generates the Rect to bind the sub-interactables to
	SDL_Rect bindingRect = genBindingRect();
	//Binds all interactables to the Width
	for (Interactable* i : interactables) {
		binded = i->bindToArea(bindingRect);
		//Exit if there was an error
		if (binded) break;
	}

	return binded;
}

/*
* Renders the interactables onto the Container Interactable
* 
* @return int, 0 on success, otherwise an error
*/
int ContainerInteractable::renderInteractables() {
	//Gets the render target
	SDL_Texture* currentTarget = SDL_GetRenderTarget(Display::getRenderer());

	//Sets the new render target
	SDL_SetRenderTarget(Display::getRenderer(), renderTexture);

	SDL_Color color = getPrimaryColor();
	//Use a clear color
	SDL_SetRenderDrawColor(Display::getRenderer(), color.r, color.g, color.b, color.a);
	//Clears the render Texture
	SDL_RenderClear(Display::getRenderer());

	//Renders the interactables to the texture
	InteractableManager::render();

	//Sets the render target back to what it was
	return SDL_SetRenderTarget(Display::getRenderer(), currentTarget);
}

/*
* Default Constructor for Container Interactable
*/
ContainerInteractable::ContainerInteractable() 
: InteractableManager(), Interactable() {
	init();
}

/*
* Deconstructor for Container Interactable
*/
ContainerInteractable::~ContainerInteractable() {
	clearTexture();
}


/*
* Adds an interactable to the Container Interactable
* 
* @param interactable, The Interactable to add
* @return true, The interactable was added
* @return false, The Interactable was not added
*/
bool ContainerInteractable::addInteractable(Interactable* interactable) {
	//If the interactable was added
	if (InteractableManager::addInteractable(interactable)) {
		invalidate();
		return true;
	}
	return false;
}

/*
* Updates the Container Interactable
* 
* @return int, 0 on success, otherwise an error occured
*/
int ContainerInteractable::update() {
	//Updates the parent
	Interactable::update();

	//Updates all the interactables
	updateInteractables();

	//If the interactable was updated
	if (getUpdated()) {
		//Binds the Interactables to the area of the container
		bindInteractablesToArea();
		//Regenerate a texture and render the interactables
		if (!generateTexture()) {
			renderInteractables();
		} else {
			std::cout << "error re-rendering!\n";
		}
	}

	return 0;
}

/*
* Updates all the interactables in the container
* 
* @return int, 0 on success, otherwise an error occured
*/
int ContainerInteractable::updateInteractables() {
	int failures = 0;
	for (Interactable* i : interactables) {
		failures += i->update() != 0;
		//Must be ReRendered if an element was updated
		if (i->getUpdated())
			invalidate();
	}
	return 0;
}

/*
* Attempts to click the Container Interactable
* 
* @param clickX, The click's X position
* @param clickY, The click's Y position
* @return int, 0 on success otherwise an error occured
*/
int ContainerInteractable::click(int clickX, int clickY) {
	//Ensures the click overlaps before calling the parent function
	if (!getPositionOverlap(clickX, clickY)) return 0;

	return InteractableManager::click(clickX - getX(), clickY - getY());
}


/*
* On Mouse Down, will allow the sub-interactables to respond
*
* @param downX, The down X position
* @param downY, The down Y position
* @return int, 0 on success otherwise an error occured
*/
int ContainerInteractable::mouseDown(int downX, int downY) {
	//Ensures the mouse down overlaps before calling the parent function
	if (!getPositionOverlap(downX, downY)) return 0;

	return InteractableManager::mouseDown(downX - getX(), downY - getY());
}

/*
* Performs the mouse scroll action on the contained interactables
*
* @param scrollX, The Mouse Scroll's X position
* @param scrollY, The Mouse Scroll's Y position
* @param scrollSpd, The Mouse Scroll's Speed
* @return int, 0 on success, otherwise an error occured
*/
int ContainerInteractable::mouseScroll(int scrollX, int scrollY, float scrollSpd) {
	//Ensures the mouse down overlaps before calling the parent function
	if (!getPositionOverlap(scrollX, scrollY)) return 0;

	return InteractableManager::mouseScroll(scrollX - getX(), scrollY - getY(), scrollSpd);
}


/*
* Generates an SDL_Rect to bind sub interactables to
*
* @return SDL_Rect, The SDL_Rect that Sub Interactables should be bound to
*/
SDL_Rect ContainerInteractable::genBindingRect() const {
	SDL_Rect genRect = getRect();

	//Defaults the position to (0, 0)
	genRect.x = 0;
	genRect.y = 0;

	return genRect;
}

/*
* Renders the Container Interactable
* 
* @param renderer, The renderer to render to
*/
void ContainerInteractable::render() {
	//Ensures the render Texture is still valid
	SDL_assert(renderTexture != nullptr);
	if (renderTexture == nullptr) return;

	//Gets the area to render the Container
	SDL_Rect renderArea = getRect();
	SDL_Color color = getPrimaryColor();

	////Draws the background for the container
	SDL_SetRenderDrawColor(Display::getRenderer(), color.r, color.g, color.b, color.a);
	SDL_RenderFillRect(Display::getRenderer(), &renderArea);

	//Renders the generated texture
	SDL_RenderCopy(Display::getRenderer(), renderTexture, NULL, &renderArea);
	
	//Revalidates the container as it has been rendered
	revalidate();
}




/*
* Initializes the List Interactable
*/
void ListInteractable::init() {
	setScrollDist(0);
	setMaxScrollDist(0);
}

/*
* Default Constructor for the List Interactable
*/
ListInteractable::ListInteractable() : ContainerInteractable() {
	init();
}

/*
* Deconstructor
* Nothing to delete :)
*/
ListInteractable::~ListInteractable() {}


/*
* Sets the Scroll distance
* 
* @param scrollDistance, the position to scroll to
* @return int, 0 on success, otherwise an error occured
*/
int ListInteractable::setScrollDist(int scrollDistance) {
	//Sets the scroll distance within valid values
	if (scrollDistance < 0) scrollDistance = 0;
	if (scrollDistance > getMaxScrollDist()) scrollDistance = getMaxScrollDist();

	//Sets the scrollDistance
	this->scrollDistance = scrollDistance;
	return 0;
}

/*
* Sets the max scroll Distance
* 
* @param maxScrollDist, The max distance that the list should be able to scroll
* @return int, 0 on success, otherwise an error occured
*/
int ListInteractable::setMaxScrollDist(int maxScrollDist) {
	//Ensures the max scroll distance isn't invalid
	SDL_assert(maxScrollDist >= 0);
	if (maxScrollDist < 0) return -1;
	//Sets the new maxScrollDistance
	this->maxScrollDistance = maxScrollDist;
	return 0;
}

/*
* Generates an SDL_Rect to bind sub interactables to
*
* @return SDL_Rect, The SDL_Rect that Sub Interactables should be bound to
*/
SDL_Rect ListInteractable::genBindingRect() const {
	SDL_Rect genRect = getRect();

	//Defaults the position to (0, 0)
	genRect.x = 0;
	genRect.y = scrollDistance;

	return genRect;
}



/*
* Gets how far the mouse is currently scrolled
* 
* @return int, the current scroll Distance
*/
int ListInteractable::getScrollDist() const { return scrollDistance; }

/*
* Gets the maximum scroll distance
* 
* @return int, the maximum scroll Distance
*/
int ListInteractable::getMaxScrollDist() const { return maxScrollDistance; }

/*
* What to do when the mouse is scrolled on the Interactable
*
* @param scrollX, The Mouse Scroll's X position
* @param scrollY, The Mouse Scroll's Y position
* @param scrollSpd, The Mouse Scroll's Speed
* @return int, 0 on success, otherwise an error occured
*/
int ListInteractable::mouseScroll(int scrollX, int scrollY, float scrollSpd) {
	//If the scroll overlaps
	if (getPositionOverlap(scrollX, scrollY)) {
		//Alters the scroll Distance
		setScrollDist(getScrollDist() + (int)scrollSpd);
		invalidate();
	}

	return 0;
}

