#pragma once



#include "SDL_rect.h"
#include "SDL.h"

#include <string>
#include <vector>


/*
* The style the texture will be rendered in 
* IE: Centered, From the Top left, etc
*/
enum class RenderStyle {
    None,
    TopLeft, // Top Left
    Centered
};

/**
 * @brief What type of Coordinate are we using
 */
enum class CoordinateType {
    None,
    Pixel,
    PixelFromRightEdge,
    PixelFromBottomEdge,
    Percentage,
    PercentageWidth,
    PercentageHeight,
    PercentageSmallest,
} typedef CordType;

/**
 * @brief Coordinate class
 * Used for the coordinates of the UI elements
 */
class Coordinate {
private:
    //The type of coordinate this is
    CordType type;
    //Stores the values in the same position
    float value;

public:
    //Default Constructor
    Coordinate();

    //Makes a coordinate with a percentage position
    Coordinate(float, CoordinateType);

    //Makes a coordinate with a pixel position
    Coordinate(int);

    /// Setters
    //Set the value of this coordinate
    bool setValue(CordType, float);

    /// Getters

    //Gets the type of coordinate we are using
    CordType getType() const;

    //Gets the value from the Coordinate
    float getValue() const;

    //Gets the pixel position of the Coordinate
    int getPixelPos(bool) const;

    //Gets the pixel position with custom Width / Height
    int getPixelPosEx(bool, int, int) const;

    //Gets the percentage Position of the Coordinate
    float getPercentagePos(bool) const;

    //Gets the percentage position of the Coordinate with custom Width / Height
    float getPercentagePosEx(bool, int, int) const;
} typedef Cord;


/**
 * @brief The superclass of all interactable elements
 */
class Interactable {
private:
    //ID
    static int IDGenerator;
    int ID;

    //The previous area this was rendered (To identify if it was updated)
    SDL_Rect prevArea;

    //Used for binding Interactables to be within one-another
    bool isBound;
    //The boundRect
    SDL_Rect boundRect;

    //If the interactable just updated
    bool updated;

    //The position of the UI element
    Coordinate x, y;
    //The size of the UI element
    Coordinate w, h;

    //The colors of the UI
    SDL_Color primaryColor;
    SDL_Color secondaryColor;
    SDL_Color tertiaryColor;

    //The render style of the interactable
    RenderStyle style;

    //Initializes the Interactable
    void init();


public:
    //Create an interactable with no default position
    Interactable();

    //Create an interactable using only percentage positions
    Interactable(float, float, float, float);

    //Create an interactable using only pixel positions
    Interactable(int, int, int, int);

    //Deconstructor (Virtual for child classes)
    virtual ~Interactable();

    //Updating the interactable
    virtual int update();
   
    /// Binding within another Interactable
    /// Binding means you are forcing an Interactable to render within a certain size!
    
    //Binds an Interactable to a size
    virtual int bindToSize(int, int);

    //Binds an Interactable to an area
    virtual int bindToArea(SDL_Rect);

    //Unbinds the Interactable
    virtual void unbind();

    /// Setters

    //Sets the primary Color
    int setPrimaryColor(Uint8, Uint8, Uint8, Uint8);
    //Sets the secondary color
    int setSecondaryColor(Uint8, Uint8, Uint8, Uint8);
    //Sets the terniary color
    int setTertiaryColor(Uint8, Uint8, Uint8, Uint8);

    //Sets the style to render in
    int setRenderStyle(RenderStyle);

    //Set the X position
    int setX(CordType, float);
    //Set the Y position
    int setY(CordType, float);
    //Set the Width
    int setW(CordType, float);
    //Set the Height
    int setH(CordType, float);

    /// Getters

    //Generates an SDL_Rect for drawing to the display
    virtual SDL_Rect getRect() const;

    //Gets the primary Color
    SDL_Color getPrimaryColor() const;
    //Gets the secondary color
    SDL_Color getSecondaryColor() const;
    //Gets the terniary color
    SDL_Color getTertiaryColor() const;

    //Gets the style of the image
    RenderStyle getRenderStyle() const;

    //Gets the X position
    int getX() const;
    //Gets the Y positon
    int getY() const;
    //Gets the Width
    int getW() const;
    //Gets the Height
    int getH() const;

    //Checks if it's bound
    bool getIsBound() const;
    //Gets the boundry from the Interactable
    SDL_Rect getBoundry() const;

    //Gets the X Coord of the Bounds
    int getBoundX() const;
    //Gets the Y Coord of the Bounds
    int getBoundY() const;
    //Gets the Bound width
    int getBoundWidth() const;
    //Gets the Bound Height
    int getBoundHeight() const;

    //Gets the ID
    int getID() const;

    //Checks if the Interactable was just updated
    bool getUpdated() const;

    //Checks if a click overlaps with the interactable
    bool getPositionOverlap(int, int) const;

    /// Interactivity

    //Attempts to click the Interactable
    virtual int click(int, int);

    //Interacts with the Interactable when the mouse is down
    virtual int mouseDown(int, int);

    //What to do when the mouse is scrolled on the interactable
    virtual int mouseScroll(int, int, float);

    /// Rendering

    //Renders the interactable
    virtual void render();

    //Forces the Interactable to be considered "Updated" thus requiring a re-render
    void invalidate();

    /**
     * Revalidates the Interactable
     * This means that if it's in a container, 
     * The container will no longer re-render itself
    */
    void revalidate();
};

/*
* @brief TextureInteractable displays a texture
*/
class TextureInteractable : virtual public Interactable {
private:
    //The Path to the texture
    std::string path;

    //The texture to render
    SDL_Texture* texture;

    //Initializes the Texture Interactable
    void init();
public:
    //Default Constructor
    TextureInteractable();

    //Constructor that loads the texture
    TextureInteractable(std::string, RenderStyle);

    //Deconstructor
    ~TextureInteractable();

    /// Setters


    //Set the texture to this image
    int setTexture(std::string);

    /// Getters

    //Gets the Path to the texture
    std::string getTexturePath() const;

    /// Clearing

    //Clears the texture from the Texture Interactable
    virtual void clearTexture();

    /// Renderering

    //Renders the TextureInteractable
    void render();
};

/**
 * @brief TextInteractable displays text, child of Interactable class
 */
class TextInteractable : virtual public Interactable {
private:
    //The text displayed
    std::string text;

    //The color of the Text
    SDL_Color textColor;

    //The size of the text to be rendered
    int textWidth;
    int textHeight;

    //The Texture to render
    SDL_Texture* text_img;

    //Initializes the Text Interactable
    void init();
public:
    //Default constructor
    TextInteractable();

    //Create a text interactable using only percentage positions
    TextInteractable(std::string, float, float, float, float);

    //Create a text interactable using only pixel positions
    TextInteractable(std::string, int, int, int, int);

    //Deconstructor 
    ~TextInteractable();

    //Updates the text interactable
    int update();

    /// Setters

    //Sets the color
    int setTextColor(SDL_Color);

    //Sets the color using typed values
    int setTextColor(Uint8, Uint8, Uint8, Uint8);

    //Sets the text to display
    bool setText(std::string);

    /// Getters

    //Gets the Text Color
    SDL_Color getTextColor() const;

    //Gets the text displayed
    std::string getText() const;

    //Gets the width of the rendered text
    int getTextWidth() const;

    //Gets the height of the rendered text
    int getTextHeight() const;

    /// Clearing

    //Clears the text
    void clearTextTexture();

    /// Rendering

    //Renders the TextInteractable
    void render();
};




/**
 * @brief Manages all the interactables within one class
 */
class InteractableManager {
protected:
    //Holds all the interactables
    std::vector<Interactable*> interactables;
private:
    //Initializes the manager
    void init();
public:
    //Constructs a new interactable manager
    InteractableManager();

    //Destructor
    virtual ~InteractableManager();

    //Adds an Interactable to the manager
    virtual bool addInteractable(Interactable*);

    //Removes an Interactable from the manager using the ID
    bool removeInteractableByID(int);

    //Removes all interactables from the manager
    void clear();

    //Updates all the interactables
    int updateInteractables();

    /// Getters 

    //Gets an interactable by its ID
    const Interactable* getInteractableByID(int) const;

    //Gets an interactable mutably by its ID
    Interactable* getInteractableByIDMut(int) const;

    /// Interactivity

    //Attempts to click on an interactable
    virtual int click(int, int);

    //Attemps to interact with the manager when the mouse is down
    virtual int mouseDown(int, int);

    //Performs the mouse scroll action on the contained interactables
    virtual int mouseScroll(int, int, float);

    /// Rendering

    //Render all interactables
    virtual void render();
};