
//SDL Libraries
#include "SDL.h"
#include "SDL_syswm.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
#undef main

#include <iostream>
#include <random>

#include <time.h> 
#include "Globals/Font.h"
#include "Globals/Display.h"

#include "Music/MusicLoader/MusicLoader.h"
#include "Music/MusicPlayer/MusicPlayer.h"
#include "Interactables/Interactables.h"
#include "Interactables/ListInteractables.h"
#include "Music/MusicDisplayer/MusicDisplayer.h"    //Displaying music
#include "MouseController/MouseController.h"    //Mouse events


int main() {
    //Initializing SDL and it's subsets
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        std::cout << "Error initializing SDL2 " << SDL_GetError();
    
    int imageFlags = IMG_INIT_PNG;
    if (IMG_Init(imageFlags) != imageFlags) {
        std::cout << "SDL_Image Not Initialized" << IMG_GetError() << std::endl;
    }

    int musicFlags = MIX_INIT_MP3;
    if (Mix_Init(musicFlags) != musicFlags)
        std::cout << "SDL_Mixer Not Initialized" << Mix_GetError() << std::endl;

    //Making the window
    if (!Display::init(500, 500)) {
        return 1;
    }

    /// Setting up The window with SDL functions
    
    //Allows for Alpha
    SDL_SetRenderDrawBlendMode(Display::getRenderer(), SDL_BLENDMODE_BLEND);

    //Sets the minimum window size
    SDL_SetWindowMinimumSize(Display::getWindow(), 400, 400);

    //Initializing the MusicPlayer
    MusicPlayer::init();
    MusicPlayer::setVolumeLinear(0.3);
    
    //Initializes the Music Loader
    MusicLoader::init();
    MusicLoader::getMusicListFromFolder("Music");

    //Interupt to delete music whenever it finishs playing
    Mix_HookMusicFinished(MusicPlayer::deleteMusicTrack);

    //Initializing the Font
    Font::init();
    Font::loadFont("Fonts/OpenSans-Bold.ttf", FontName::UIFont);

    //The interactable manager
    InteractableManager* interactableManager = new InteractableManager();

    /// Bottom Audio Controls Container

    ContainerInteractable* container = new ContainerInteractable();

    //Sets the Color
    container->setPrimaryColor(50, 50, 50, 255);
    container->setX(CordType::PercentageWidth, 0.5f);
    container->setY(CordType::PixelFromBottomEdge, 75);
    container->setW(CordType::Pixel, 400);
    container->setH(CordType::Pixel, 125);
    //Centers the Container
    container->setRenderStyle(RenderStyle::Centered);

    interactableManager->addInteractable(container);

    ///Play / Pause button   

    //Makes the pauseButton
    PlayPauseInteractable* pauseButton = new PlayPauseInteractable();

    //Sets the position of the pauseButton
    pauseButton->setX(CordType::PercentageWidth, 0.5);
    pauseButton->setY(CordType::Pixel, 40);
    pauseButton->setW(CordType::Pixel, 50);
    pauseButton->setH(CordType::Pixel, 50);

    container->addInteractable(pauseButton);

    /// Skip forward button

    SkipForwardInteractable* skipForwardButton = new SkipForwardInteractable();

    //Sets the position of the Skip forward
    skipForwardButton->setX(CordType::PercentageWidth, 0.75);
    skipForwardButton->setY(CordType::Pixel, 40);
    skipForwardButton->setW(CordType::Pixel, 50);
    skipForwardButton->setH(CordType::Pixel, 50);

    container->addInteractable(skipForwardButton);

    /// Skip backward button

    SkipBackwardInteractable* skipBackwardButton = new SkipBackwardInteractable();

    //Sets the position of the Skip forward
    skipBackwardButton->setX(CordType::PercentageWidth, 0.25);
    skipBackwardButton->setY(CordType::Pixel, 40);
    skipBackwardButton->setW(CordType::Pixel, 50);
    skipBackwardButton->setH(CordType::Pixel, 50);

    container->addInteractable(skipBackwardButton);

    /// Volume Interactable

    //Makes the Volume interactable
    VolumeInteractable* volume = new VolumeInteractable();

    //Sets the position of the volume
    volume->setX(CordType::PercentageWidth, 0.5);
    volume->setY(CordType::Pixel, 100);
    volume->setW(CordType::Pixel, 300);
    volume->setH(CordType::Pixel, 40);
    //Centers the volume
    volume->setRenderStyle(RenderStyle::Centered);

    container->addInteractable(volume);

    /// Music List

    MusicListInteractable* musicList = new MusicListInteractable();
    
    musicList->setX(CordType::PercentageWidth, 0);
    musicList->setY(CordType::PercentageHeight, 0);
    musicList->setW(CordType::PercentageWidth, 1);
    musicList->setH(CordType::PixelFromBottomEdge, 150);

    musicList->setRenderStyle(RenderStyle::None);
    musicList->addMusic(MusicLoader::getSongData());

    interactableManager->addInteractable(musicList);

    //Sets the window Icon
    SDL_Surface* icon = IMG_Load("Clef.png");
    SDL_SetWindowIcon(Display::getWindow(), icon);

    //Main loop
    while (!Input::getExit()) {
        //Clears the window
        Display::clear();

        //Updates the input handler
        Input::update(interactableManager);

        //Updates the UI
        interactableManager->updateInteractables();
        interactableManager->render();

        //Displays the window
        Display::render();
    }


    //Properly destroys all UI elements
    interactableManager->clear();
    delete interactableManager;

    //Close the music player
    MusicPlayer::close();
    //Closes the musicLoader
    MusicLoader::close();

    //Close the font librrary
    Font::close();
    //Closes the display
    Display::quit();

    //Close Libraries
    IMG_Quit();
    Mix_Quit();
    SDL_Quit();
}