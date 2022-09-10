#pragma once

#include <SDL_ttf.h>
#include <string>


//The font used by the font handler
enum class FontName {
    UIFont,
};

namespace Font {
    //Initializes the font system
    bool init();

    //Closes the font system
    void close();

    //Checks that the font system is loaded
    bool loaded();

    //Loads a font to the file path, and stores it as the FontName
    bool loadFont(std::string path, FontName name);

    //Gets the font with said name
    const TTF_Font* getFontByName(FontName name);

    //Get the font mutably with said name
    TTF_Font* getFontByNameMut(FontName name);
}