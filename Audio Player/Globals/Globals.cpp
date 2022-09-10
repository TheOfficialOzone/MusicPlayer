

#include "Globals.h"
#include <iostream>

#include "Display.h"
#include "SDL_image.h"

/*
* Loads a texture from the file
* 
* @param path, The path to the texture
* @return SDL_Texture*, pointer to the texture, nullptr on error
*/
SDL_Texture* loadTextureFromFile(std::string path) {
	//Load the image
	SDL_Surface* surface = IMG_Load(path.c_str());
	SDL_Texture* texture = nullptr;

	//If the surface failed to load
	if (surface == nullptr) { return nullptr; }

	//Generate the texture
	texture = SDL_CreateTextureFromSurface(Display::getRenderer(), surface);

	//Surface can now be freed
	SDL_FreeSurface(surface);
	surface = nullptr;

	//Return the texture whether or not it worked
	return texture;
}