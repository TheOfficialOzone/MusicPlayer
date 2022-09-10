


#include "display.h"

#include <SDL.h>
#include <SDL_assert.h>

static bool displayReady = false;
static SDL_Window* window = nullptr;
static SDL_Renderer* renderer = nullptr;


static int prevWidth;
static int prevHeight;
static bool sizeChanged;

/**
 * @brief Initializes the display
 */
bool Display::init(unsigned int width, unsigned int height) {
    //Ensures that the display isn't already loaded
    SDL_assert(!displayReady);
    if (displayReady) return true;

    //Creates the window
    window = SDL_CreateWindow("Audio Player", 200, 200, width, height, SDL_WINDOW_RESIZABLE);
    
    //Creates the renderer
    if (window != nullptr)
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    //Updates the previous width and height of the display
    prevWidth = width;
    prevHeight = height;
    //The size just changed
    sizeChanged = true;

    displayReady = (window != nullptr && renderer != nullptr);
    return displayReady;
}

/**
 * @brief Quits the display Process
 */
void Display::quit() {
    //Ensures the display is ready before closing
    SDL_assert(displayReady);

    //Destroys the renderer
    if (renderer != nullptr) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }

    //Destroys the window
    if (window != nullptr) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    //No longer good to display
    displayReady = false;
}


/**
 * @brief Gets the window from the display
 *
 * @return SDL_Window* The pointer to the Displays Window
 */
SDL_Window* Display::getWindow() {
    SDL_assert(displayReady);
    return window;
}

/**
 * @brief Gets the renderer
 *
 * @return SDL_Renderer* The pointer to the Displays Renderer
 */
SDL_Renderer* Display::getRenderer() {
    SDL_assert(displayReady);
    return renderer;
}

/**
 * @brief Gets the size of the display
 *
 * @param w filled with the width of the display
 * @param h filled with the height of the display
 */
void Display::getSize(int& w, int& h) {
    SDL_assert(displayReady);
    SDL_GetWindowSize(window, &w, &h);
}


/**
 * @brief Gets the width of the windows
 *
 * @return int The width of the display
 */
int Display::getWidth() {
    //Checks the the display is ready
    SDL_assert(displayReady);

    //Get the size from the Display
    int width, height;
    getSize(width, height);
    //Return the width
    return width;
}

/**
 * @brief Gets the height of the windows
 *
 * @return int The height of the display, -1 on error
 */
int Display::getHeight() {
    //Checks the the display is ready
    SDL_assert(displayReady);
    if (!displayReady) return -1;

    //Get the size from the Display
    int width, height;
    getSize(width, height);
    //Return the height
    return height;
}

/*
* Gets whether or not the size of the display just changed
* 
* @return true, The size of the display changed
* @return false, The size of the display was not changed
*/
bool Display::getSizeChanged() { return sizeChanged; }

/// Updating

/// Rendering

/*
* Clears the display (Defaults to a grey for this project)
* 
* @return int, 0 on success, otherwise an error occured
*/
int Display::clear() {
    //Checks the the display is ready
    SDL_assert(displayReady);
    if (!displayReady) return -1;

    SDL_SetRenderDrawColor(Display::getRenderer(), 100, 100, 100, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(Display::getRenderer());

    return 0;
}

/*
* Renders the display to the screen
* 
* @return int, 0 on success, otherwise an error
*/
int Display::render() {
    //Checks the the display is ready
    SDL_assert(displayReady);
    if (!displayReady) return -1;

    //The size just changed
    sizeChanged = false;

    //Gets the current width and Height
    int currWidth = getWidth();
    int currHeight = getHeight();

    //Checks if the size changed
    sizeChanged = (currWidth != prevWidth) || (currHeight != prevHeight);

    //Updates the previous width and height of the display
    prevWidth = currWidth;
    prevHeight = currHeight;

    //If the size changed exit as everything may be rendered incorrectly!
    if (sizeChanged) return 0;

    //Renders the current Renderer
    SDL_RenderPresent(Display::getRenderer());

    return 0;
}