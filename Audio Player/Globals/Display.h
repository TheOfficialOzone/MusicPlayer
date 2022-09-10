#pragma once


struct SDL_Renderer;
struct SDL_Window;

namespace Display {
    //Creates the display
    bool init(unsigned int width, unsigned int height);

    //Quits the display
    void quit();

    /// Getters

    //Gets the Renderer used by the display
    SDL_Renderer* getRenderer();
    //Gets the Window used by the display
    SDL_Window* getWindow();

    //Gets the size of the display
    void getSize(int&, int&);

    //Gets the width of the display
    int getWidth();

    //Gets the height of the display
    int getHeight();

    //Gets whether or not the size of the display just changed
    bool getSizeChanged();

    /// Rendering

    //Clears the display
    int clear();

    //Renders the display to the screen
    int render();
}