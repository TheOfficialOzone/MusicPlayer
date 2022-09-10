#pragma once



#include <random>
#include <string>
#include "SDL.h"
#include "SDL_ttf.h"

//Loads a texture from the file
SDL_Texture* loadTextureFromFile(std::string path);
