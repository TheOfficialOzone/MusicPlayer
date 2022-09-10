#include "font.h"

#include <map>
#include <iostream>

#include <SDL_assert.h>
#include <SDL_ttf.h>

#define RESOLUTION 50

static bool fontInitialized = false;
static std::map<FontName, TTF_Font*> fontMap;

/**
 * @brief Initializes the font system
 *
 * @return true The font system was initialized
 * @return false There was an error initializing the fonts
 */
bool Font::init() {
    //Fonts shouldn't be initialized / true if already initialized
    SDL_assert(!loaded());
    if (loaded()) return true;

    //Loads TTF
    fontInitialized = (TTF_Init() == 0);
    return fontInitialized;
}

/**
 * @brief Closes the font handler
 */
void Font::close() {
    //Fonts should be loaded before closing the font handler
    SDL_assert(loaded());
    if (!loaded()) return;

    //No longer loaded
    fontInitialized = false;

    //Quits TTF
    TTF_Quit();
}

/*
* Checks that the font system is loaded
* 
* @return bool, True if the font system is loaded
*/
bool Font::loaded() {
    return fontInitialized;
}


/**
 * @brief Loads a font into the font handler
 *
 * @param path The path of the font
 * @param name The name the font is stored under
 * @return true The Font was loaded
 * @return false No Font was found / The font system is not loaded
 */
bool Font::loadFont(std::string path, FontName name) {
    //Ensures that the fonts are loaded
    SDL_assert(loaded());
    if (!loaded());

    //Load the path
    TTF_Font* newFont;
    //Loads the font at the specified resolution
    newFont = TTF_OpenFont(path.c_str(), RESOLUTION);

    //Checks that the font was loaded
    if (newFont == nullptr) {
        std::cout << "Error loading font in path: " << path << std::endl;
        return false;
    }

    //Inserts the font into the map
    fontMap.insert(std::pair<FontName, TTF_Font*>(name, newFont));
    return true;
}

/**
 * @brief Gets the font with the specified name
 *
 * @param name The name of the font
 * @return const TTF_Font* The font with the name, nullptr if no font with that name exists
 */
const TTF_Font* Font::getFontByName(FontName name) {
    return fontMap.at(name);
}

/**
 * @brief Gets the font with the specified name
 *
 * @param name The name of the font
 * @return const TTF_Font* The font with the name, nullptr if no font with that name exists
 */
TTF_Font* Font::getFontByNameMut(FontName name) {
    return fontMap.at(name);
}