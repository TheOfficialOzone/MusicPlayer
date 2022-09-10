#include <vector>
#include <iostream>
#include <string>

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_assert.h"

#include "Globals/Globals.h"
#include "Globals/Math.h"
#include "Globals/Display.h"
#include "Globals/Font.h"
#include "Interactables.h"

//Default the ID generator to 0
int Interactable::IDGenerator = 0;

/**
 * @brief Construct a new Coordinate object
 * NOTE: Defaults Coordinate CordType to None, will need to be overwritten
 */
Coordinate::Coordinate() {
    setValue(CordType::None, -1);
}

/**
 * @brief Construct a new Coordinate object
 *
 * @param value The percentage position of the coordinate
 */
Coordinate::Coordinate(float value, CordType type) : type(type), value(value) {}

/**
 * @brief Construct a new Coordinate object
 *
 * @param value The pixel position of the coordinate
 */
Coordinate::Coordinate(int value) : type(CordType::Pixel), value(value) {}


/**
 * @brief Sets the value of the coordinate
 *
 * @param type The type of coordinate this is
 * @param value The value stored within the Coordinate
 * @return true, The values were changed (Always TRUE atm)
 * @return false, The values weren't changed (Never occurs atm)
 */
bool Coordinate::setValue(CordType type, float value) {
    this->type = type;
    this->value = value;
    return true;
}

/**
 * @brief Gets the type of Coordinate we are using
 *
 * @return CordType The type of coordinate
 */
CordType Coordinate::getType() const { return type; }

/**
 * @brief Gets the RAW value from the Coordinate object
 *
 * @return float The value stored within the Coordinate object
 */
float Coordinate::getValue() const { return value; }


/**
 * @brief Gets the pixel Position from the Coordinate object
 *
 * @param useWidth Should the pixel pos be based off the width (True) or Height (False)
 * @return int The Pixel Position of the Coordinate, -1 if no value set
 */
int Coordinate::getPixelPos(bool useWidth) const {
    return getPixelPosEx(useWidth, Display::getWidth(), Display::getHeight());
}

/**
 * @brief Gets the pixel Position from the Coordinate object
 *
 * @param useWidth, Should the pixel pos be based off the width (True) or Height (False)
 * @param width, The width
 * @param height, The height
 * @return int The Pixel Position of the Coordinate, -1 if no value set
 */
int Coordinate::getPixelPosEx(bool useWidth, int width, int height) const {
    int pixel = -1;
    switch (type) {
        // Use the value already stored
    case CordType::Pixel:
        //Get the displays Width / Height over the pixel position
        pixel = (int)getValue();
        break;

        //Use the value stored as an offset from the right edge
    case CordType::PixelFromRightEdge:
        pixel = width - (int)getValue();
        break;

        //Use the value stored as an offset from the bottom edge
    case CordType::PixelFromBottomEdge:
        pixel = height - (int)getValue();
        break;
        // Calculate the pixel position from the size
    case CordType::Percentage:
        //Use the Width / Height times the percentage across
        pixel = (useWidth ? width : height);
        pixel = std::round(pixel * getValue());
        break;
        // Calculate the pixel position from the width
    case CordType::PercentageWidth:
        pixel = std::round(width * getValue());
        break;
        // Calculate the pixel position from the height
    case CordType::PercentageHeight:
        pixel = std::round(height * getValue());
        break;
        // Calculate the pixel position from the smaller of the size
    case CordType::PercentageSmallest:
        pixel = (width < height ? width : height);
        pixel = std::round(pixel * getValue());
        break;
        //Default to -1
    default:
        pixel = -1;
        break;
    }
    return pixel;
}

/**
 * @brief Gets the percentage Position from the Coordinate object
 *
 * @param useWidth Should the percentage be based off the width (True) or Height (False)
 * @return int The Percentage Position of the Coordinate, -1 if no value set
 */
float Coordinate::getPercentagePos(bool useWidth) const {
    return getPercentagePosEx(useWidth, Display::getWidth(), Display::getHeight());
};

/**
 * @brief Gets the percentage Position from the Coordinate object
 *
 * @param useWidth Should the percentage be based off the width (True) or Height (False)
 * @param width, The width
 * @param height, The height
 * @return int The Percentage Position of the Coordinate, -1 if no value set
 */
float Coordinate::getPercentagePosEx(bool useWidth, int width, int height) const {
    float percentage = -1;
    switch (type) {
        // Calculate the percentage from 
    case CordType::Pixel:
        //Get the Width / Height over the pixel position
        percentage = (useWidth ? width : height);
        percentage = getValue() / percentage;
        break;

        //Get the pixel position as a percentage of the width
    case CordType::PixelFromRightEdge:
        percentage = getPixelPosEx(useWidth, width, height) / width;
        break;

        //Get the pixel position as a percentage of the height
    case CordType::PixelFromBottomEdge:
        percentage = getPixelPosEx(useWidth, width, height) / height;
        break;
        // Use the value already stored
    case CordType::Percentage:
    case CordType::PercentageWidth:
    case CordType::PercentageHeight:
    case CordType::PercentageSmallest:
        percentage = getValue();
        break;
        // Default to -1
    default:
        percentage = -1;
        break;
    }
    return percentage;
}


/**
 * @brief Initializes the interactable
 */
void Interactable::init() {
    //Generates the new ID
    ID = IDGenerator++;
    //Defaults the color to white
    setPrimaryColor(50, 50, 50, 255);
    setSecondaryColor(25, 25, 25, 255);
    setTertiaryColor(0, 0, 0, 255);
    //Defaults the binding
    unbind();
}


/**
 * @brief Construct a new Interactable object
 */
Interactable::Interactable() { init(); }

/**
 * Revalidates the Interactable,
 * This means that if it's in a container, 
 * The container will no longer re-render itself
 */
void Interactable::revalidate() {
    updated = false;
}

/**
 * @brief Construct a new Interactable object
 * (Percentage wise...)
 * @param x The x percentage position
 * @param y The y percentage position
 * @param w The width percentage size
 * @param h The height percentage size
 */
Interactable::Interactable(float x, float y, float w, float h) {
    init();
    //Set the values of each Coordinate
    this->x.setValue(CordType::Percentage, x);
    this->y.setValue(CordType::Percentage, y);
    this->w.setValue(CordType::Percentage, w);
    this->h.setValue(CordType::Percentage, h);
}

/**
 * @brief Construct a new Interactable object
 * (Pixel wise...)
 * @param x The x pixel position
 * @param y The y pixel position
 * @param w The width pixel size
 * @param h The height pixel size
 */
Interactable::Interactable(int x, int y, int w, int h) {
    init();
    //Set the values of each Coordinate
    this->x.setValue(CordType::Pixel, x);
    this->y.setValue(CordType::Pixel, y);
    this->w.setValue(CordType::Pixel, w);
    this->h.setValue(CordType::Pixel, h);
}


/**
 * @brief Destroy the Interactable object
 */
Interactable::~Interactable() {}


/*
* Updates the interactable
* NOTE: If overriding this function in a child class, This must still be called
* 
* @return int, 0 on success
*/
int Interactable::update() {
    //Gets the current Area
    SDL_Rect currentArea = getRect();

    //Checks if the previous area and current area are different
    if (!compare(currentArea, prevArea) || Display::getSizeChanged()) {
        //Then it gets updated
        invalidate();
        prevArea = currentArea;
    }

    return 0;
}

/*
* Binds an Interactable to a certain size
* 
* @param width, The width to bind the Interactable to
* @param height, The height to bind the Interactable to
* @return int, 0 on success, otherwise an error occured
*/
int Interactable::bindToSize(int width, int height) {
    //Ensures the width / height is valid
    SDL_assert(width > 0 && height > 0);
    if (width <= 0 || height <= 0) return 1;

    //Binds the Interactable
    boundRect.x = 0;    //Defaults the position to (0,0)
    boundRect.y = 0;
    boundRect.w = width;
    boundRect.h = height;
    isBound = true;
    //Invalidates the interactable
    invalidate();
    //Successfully bound the Interactable
    return 0;
}

/*
* Binds an Interactable to an area
* 
* @param area, The area to bind the Interactable to
* @return int, 0 on success, otherwise an error
*/
int Interactable::bindToArea(SDL_Rect area) {
    //Ensures the width / height of the area is valid
    SDL_assert(area.w > 0 && area.h > 0);
    if (area.w <= 0 || area.h <= 0) return 1;

    //Binds the Interactable
    boundRect = area;
    isBound = true;
    //Invalidates the interactable
    invalidate();
    //Successfully bound the Interactable
    return 0;
}

/*
* Unbinds the Interactable
*/
void Interactable::unbind() {
    boundRect.x = 0;
    boundRect.y = 0;
    boundRect.w = -1;
    boundRect.h = -1;
    isBound = false;

    //Invalidates the interactable
    invalidate();
}


/*
* Sets the primary color
*
* @param r, Red
* @param g, Green
* @param b, Blue
* @param a, Alpha
* @return int, 0 on success, otherwise an error occured
*/
int Interactable::setPrimaryColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    //Sets the color
    primaryColor.r = r;
    primaryColor.g = g;
    primaryColor.b = b;
    primaryColor.a = a;
    //Invalidates the Interactable since the color is different
    invalidate();

    return 0;
}

/*
* Sets the secondary color
*
* @param r, Red
* @param g, Green
* @param b, Blue
* @param a, Alpha
* @return int, 0 on success, otherwise an error occured
*/
int Interactable::setSecondaryColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    //Sets the color
    secondaryColor.r = r;
    secondaryColor.g = g;
    secondaryColor.b = b;
    secondaryColor.a = a;
    //Invalidates the Interactable since the color is different
    invalidate();

    return 0;
}

/*
* Sets the tertiary color
*
* @param r, Red
* @param g, Green
* @param b, Blue
* @param a, Alpha
* @return int, 0 on success, otherwise an error occured
*/
int Interactable::setTertiaryColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    //Sets the color
    tertiaryColor.r = r;
    tertiaryColor.g = g;
    tertiaryColor.b = b;
    tertiaryColor.a = a;
    //Invalidates the Interactable since the color is different
    invalidate();

    return 0;
}


/*
* Sets the style of the image
*
* @param style, The style to use
* @return int, 0 on success, otherwise an error
*/
int Interactable::setRenderStyle(RenderStyle style) {
    this->style = style;
    invalidate();
    return 0;
}

/*
* Set the X position
* 
* @param type, The type of value
* @param value, The value
* @return int, 0 on success
*/
int Interactable::setX(CordType type, float value) {
    this->x.setValue(type, value);
    invalidate();
    return 0;
}

/*
* Set the Y position
*
* @param type, The type of value
* @param value, The value
* @return int, 0 on success
*/
int Interactable::setY(CordType type, float value) {
    this->y.setValue(type, value);
    invalidate();
    return 0;
}

/*
* Set the Width
*
* @param type, The type of value
* @param value, The value* 
* @return int, 0 on success
*/
int Interactable::setW(CordType type, float value) {
    this->w.setValue(type, value);
    invalidate();
    return 0;
}

/*
* Set the Height
*
* @param type, The type of value
* @param value, The value
* @return int, 0 on success
*/
int Interactable::setH(CordType type, float value) {
    this->h.setValue(type, value);
    invalidate();
    return 0;
}

/**
 * @brief Gets the SDL_Rect of the interactab;e
 *
 * @return SDL_Rect The rectangle where the interactable is rendered
 */
SDL_Rect Interactable::getRect() const {
    SDL_Rect rect;

    int boundWidth = getBoundWidth();
    int boundHeight = getBoundHeight();
    //Gets the pixel position of the rectangle
    rect.x = x.getPixelPosEx(true, boundWidth, boundHeight) - getBoundX();
    rect.y = y.getPixelPosEx(false, boundWidth, boundHeight) - getBoundY();
    rect.w = w.getPixelPosEx(true, boundWidth, boundHeight);
    rect.h = h.getPixelPosEx(false, boundWidth, boundHeight);

    //Changes the position based off the style
    switch (getRenderStyle()) {
        // Will Center the texture style
    case RenderStyle::Centered:
        rect.x -= rect.w / 2;
        rect.y -= rect.h / 2;
        break;
        //All other styles can exit
    default:
        break;
    }

    //Return the rect
    return rect;
}

/*
* Gets the primary color
* 
* @return SDL_Color, The Primary color of the Interactable
*/
SDL_Color Interactable::getPrimaryColor() const { return primaryColor; }

/*
* Gets the secondary color
*
* @return SDL_Color, The Secondary color of the Interactable
*/
SDL_Color Interactable::getSecondaryColor() const { return secondaryColor; }

/*
* Gets the tertiary color
*
* @return SDL_Color, The Tertiary color of the Interactable
*/
SDL_Color Interactable::getTertiaryColor() const { return tertiaryColor; }

/*
* Gets the style of the image
*
* @return RenderStyle, The Style of the image
*/
RenderStyle Interactable::getRenderStyle() const {
    return style;
}


/**
 * @brief Gets the X coordinate of the interactable
 *
 * @return int The pixel x of the interactable
 */
int Interactable::getX() const { 
    return getRect().x;
}

/**
 * @brief Gets the Y coordinate of the interactable
 *
 * @return int, The pixel y of the interactable
 */
int Interactable::getY() const { 
    return getRect().y;
}

/**
 * @brief Gets the width coordinate of the interactable
 *
 * @return int, The pixel width of the interactable
 */
int Interactable::getW() const { 
    return getRect().w;
}

/**
 * @brief Gets the height coordinate of the interactable
 *
 * @return int, The pixel height of the interactable
 */
int Interactable::getH() const {
    return getRect().h;
}


/*
* Checks if the Interactable is bound
* 
* @return true, The Interactable is bound
*/
bool Interactable::getIsBound() const {
    return isBound;
}

/*
* Gets the Boundry of the Interactable
* 
* @return SDL_Rect, An SDL_Rect with the boundry of the Interactable
*/
SDL_Rect Interactable::getBoundry() const {
    SDL_Rect bound;
    bound.x = getBoundX();
    bound.y = getBoundY();
    bound.w = getBoundWidth();
    bound.h = getBoundHeight();
    return bound;
}

/*
* Gets the X coordinate of the boundry
*
* @return int, The x position of the boundry
*/
int Interactable::getBoundX() const { return boundRect.x; }

/*
* Gets the Y coordinate of the boundry
* 
* @return int, The y position of the boundry
*/
int Interactable::getBoundY() const { return boundRect.y; }

/*
* Gets the Bound width
* 
* @return int, The bound width, Display::getWidth() if not bound
*/
int Interactable::getBoundWidth() const {
    //Gets the display's width
    int width = Display::getWidth();
    //If it's bound use the bound width
    if (getIsBound()) {
        width = boundRect.w;
    }
    return width;
}

/*
* Gets the Bound height
*
* @return int, The bound height, Display::getHeight() if not bound
*/
int Interactable::getBoundHeight() const {
    //Gets the display's height
    int height = Display::getHeight();
    //If it's bound use the bound height
    if (getIsBound()) {
        height = boundRect.h;
    }
    return height;
}

/**
 * @brief Gets the ID from the Interactable
 *
 * @return int The ID
 */
int Interactable::getID() const { return ID; }


/*
* Checks if the Interactable was just updated
* 
* @return true, The Interactable was updated
* @return false, The Interactable was not updated
*/
bool Interactable::getUpdated() const { return updated; }

/*
* Checks if the position overlaps with the interactable
* 
* @param posX, The X position
* @param posY, The Y position
* @return true, The click overlaps
* @return false, The click does not overlap
*/
bool Interactable::getPositionOverlap(int posX, int posY) const {
    //Gets the rect of the Interactable
    SDL_Rect rect = getRect();

    //Checks if the click overlaps with the interactable
    bool overlap = rect.x < posX && rect.y < posY;
    return overlap && rect.x + rect.w > posX && rect.y + rect.h > posY;
}


/*
* Attempts to click the interactable
*
* @param clickX, The click's X position
* @param clickY, The click's Y position
* @return int, 0 on success otherwise an error occured
*/
int Interactable::click(int clickX, int clickY) {
    return 0;
}

/*
* Interacts with the Interactable when the mouse is Down
* 
* @param downX, The Mouse Down's X position
* @param downY, The Mouse Down's Y position
* @return int, 0 on success otherwise an error occured
*/
int Interactable::mouseDown(int downX, int downY) {
    return 0;
}

/*
* What to do when the mouse is scrolled on the Interactable
* 
* @param scrollX, The Mouse Scroll's X position
* @param scrollY, The Mouse Scroll's Y position
* @param scrollSpd, The Mouse Scroll's Speed
* @return int, 0 on success, otherwise an error occured
*/
int Interactable::mouseScroll(int scrollX, int scrollY, float scrollSpd) {
    return 0;
}

/**
 * @brief Renders the interactable
 */
void Interactable::render() {
    SDL_Rect renderArea = getRect();

    SDL_Color color = getPrimaryColor();
    //Sets the color
    SDL_SetRenderDrawColor(Display::getRenderer(), color.r, color.g, color.b, color.a);
    //Renders the auto generated Rect from the interactaable
    SDL_RenderFillRect(Display::getRenderer(), &renderArea);

    //Revalidates the Interactable as it has been rendered
    revalidate();
}


/*
* Forces the Interactable to be considered "Updated" thus requiring a re-render
*/
void Interactable::invalidate() {
    updated = true;
}

/*
* Initializes the Texture Interactable
*/
void TextureInteractable::init() {
    //Defaults to an empty image
    path = "";
    texture = nullptr;
}

/*
* Defaults constructor for Texture Interactable
*/
TextureInteractable::TextureInteractable() : Interactable() {
    init();
}

/*
* Constructor that loads the texture
*
* @param path, The path to the texture
*/
TextureInteractable::TextureInteractable(std::string path, RenderStyle style) : Interactable() {
    init();
    //Sets the Texture and Style
    setTexture(path);
    setRenderStyle(style);
}


/*
* Deconstructor for the Texture Interactable
*/
TextureInteractable::~TextureInteractable() {
    clearTexture();
}


/*
* Sets the Interactables Texture
* 
* @param path, The path to the image
* @return int, 0 on success, otherwise an error
*/
int TextureInteractable::setTexture(std::string path) {
    //Clears the texture
    clearTexture();

    //Loads the texture
    texture = loadTextureFromFile(path);

    //If the texture fails to load, return 2
    if (texture == nullptr) {
        std::cout << "Error generating Texture!\n";
        return 1;
    }

    //Stores the path
    this->path = path;

    return 0;
}

/*
* Gets the path to the texture
* 
* @return string, "" if no texture is set
*/
std::string TextureInteractable::getTexturePath() const {
    return path;
}


/*
* Clears the texture from the Interactable
*/
void TextureInteractable::clearTexture() {
    //If there already is a previous texture, destroy it
    if (texture != nullptr) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
}

/*
* Renders the Texture Interactable
* 
* @param renderer, Where to render the texture to
*/
void TextureInteractable::render() {
    //Ensures that the texture is loaded before rendering
    SDL_assert(texture != nullptr);
    if (texture == nullptr) return;

    //The area to render to
    SDL_Rect renderArea = getRect();

    //Copies the texture to the display
    SDL_RenderCopy(Display::getRenderer(), texture, NULL, &renderArea);

    //Revalidates the Interactable as it has been rendered
    revalidate();
}

/*
* Initializes the Text Interactable
*/
void TextInteractable::init() {
    //Image has not been generated yet
    text_img = nullptr;
    //Defaults the textWidth to 0
    textWidth = 0;
    textHeight = 0;
    //Defaults text to "NOT SET!"
    setText("NOT SET!");
}

/**
 * @brief Construct a new Text Interactable object
 */
TextInteractable::TextInteractable() : Interactable() { init(); }


/**
 * @brief Construct a new Text Interactable object
 *
 * @param text The text to display
 * @param x The x percentage position
 * @param y The y percentage position
 * @param w The width percentage size
 * @param h The height percentage size
 */
TextInteractable::TextInteractable(std::string text, float x, float y, float w, float h)
    : Interactable(x, y, w, h) {
    init();
    setText(text);
}


/**
 * @brief Construct a new Text Interactable object
 *
 * @param text The text to display
 * @param x The x percentage position
 * @param y The y percentage position
 * @param w The width percentage size
 * @param h The height percentage size
 */
TextInteractable::TextInteractable(std::string text, int x, int y, int w, int h)
    : Interactable(x, y, w, h) {
    init();
    setText(text);
}

/*
* Deconstructor
* 
* Clears the Texture
*/
TextInteractable::~TextInteractable() {
    clearTextTexture();
}


/*
* Updates the Text Interactable
*
* @return int, 0 on success
*/
int TextInteractable::update() {
    //Calls the parent function
    Interactable::update();
    
    return (text_img == nullptr); //(Since 0 is success!)
}

/*
* Sets the Text Color
*
* @param color, The new color to use
* @return int, 0 on success
*/
int TextInteractable::setTextColor(SDL_Color color) {
    this->textColor = color;
    return 0;
}

/*
* Sets the Text color using typed values
*
* @param r, Red
* @param g, Green
* @param b, Blue
* @param a, Alpha
* @return int, 0 on success, otherwise an error occured
*/
int TextInteractable::setTextColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    //Sets the color
    textColor.r = r;
    textColor.g = g;
    textColor.b = b;
    textColor.a = a;

    return 0;
}

/**
 * @brief Sets the Text interactables Text to display
 *
 * @param text The text to display
 * @return true The text was altered, (Always true atm)
 * @return false There was an error altering the text (Never false atm)
 */
bool TextInteractable::setText(std::string text) {
    this->text = text;

    //Gets the size of the text
    TTF_SizeUTF8(Font::getFontByNameMut(FontName::UIFont), getText().c_str(), &textWidth, &textHeight);
    
    //Makes the surface
    SDL_Surface* text_surf = TTF_RenderText_Blended(Font::getFontByNameMut(FontName::UIFont), getText().c_str(), getTextColor());
     
    //Ensures the surface was generated
    if (text_surf != nullptr) {
        //Generates the image
        text_img = SDL_CreateTextureFromSurface(Display::getRenderer(), text_surf);
        //Clears the surface
        SDL_FreeSurface(text_surf);
        text_surf = nullptr;
    }

    return text_img != nullptr;
}


/*
* Gets the Text Color
* 
* @return SDL_Color, The color of the text
*/
SDL_Color TextInteractable::getTextColor() const { return textColor; }

/*
* Gets the width of the rendered text
* NOTE: getW() will return the same value, unless the width is less than the text width
* 
* @return int, The height of the rendered text
*/
int TextInteractable::getTextWidth() const { return textWidth; }

/*
* Gets the height of the rendered text
* NOTE: getH() will return the same value, unless the height is less than the text height
* 
* @return int, The height of the rendered text
*/
int TextInteractable::getTextHeight() const { return textHeight; }

/**
 * @brief Get the text
 *
 * @return std::string The text currently being displayed
 */
std::string TextInteractable::getText() const { return text; }


/*
* Clears the text texture
*/
void TextInteractable::clearTextTexture() {
    //Delete the current Text Texture
    if (text_img != nullptr) {
        SDL_DestroyTexture(text_img);
        text_img = nullptr;
    }
}

/**
 * @brief Renders the text interactable
 */
void TextInteractable::render() {    
    //Ensures that the texture image is loaded before rendering
    SDL_assert(text_img != nullptr);
    if (text_img == nullptr) return;

    //The area to render to
    SDL_Rect renderArea = getRect();

    //Corrects the renderArea for text that is smaller than the given size
    renderArea.w = getTextWidth();
    renderArea.h = getTextHeight();

    //Copies the texture to the display
    SDL_RenderCopy(Display::getRenderer(), text_img, NULL, &renderArea);

    //Revalidates the Interactable as it has been rendered
    revalidate();
}



/**
 * @brief Initializes the manager
 */
void InteractableManager::init() {}

/**
 * @brief Construct a new Interactable Manager object
 */
InteractableManager::InteractableManager() {
    init();
}

/**
 * @brief Destroy the Interactable Manager object
 */
InteractableManager::~InteractableManager() {
    clear();
}

/**
 * @brief Adds an Interactable to the manager
 *
 * @param toAdd The interactable to add
 * @return true The interactable was added to the manager
 * @return false There was an error adding the Interactable (may have passed nullptr)
 */
bool InteractableManager::addInteractable(Interactable* toAdd) {
    //Must have a valid interactable to add
    SDL_assert(toAdd != nullptr);
    if (toAdd == nullptr) return false;

    //Renders this interactable first
    interactables.push_back(toAdd);

    //The interactable was added so return true
    return true;
}

/**
 * @brief Removes an interactable based off the ID
 *
 * @param ID The ID of the interactable to remove
 * @return true The interactable was removed
 * @return false No interactable was removed
 */
bool InteractableManager::removeInteractableByID(int ID) {
    bool removed = false;

    //Loops through each interactable
    for (int i = 0; i < interactables.size(); i++) {
        //Checks if the IDs match
        if (interactables.at(i)->getID() == ID) {
            //Remove the i'th element
            delete interactables.at(i);
            interactables.erase(interactables.begin() + i);
            removed = true;
        }
    }

    return removed;
}

/**
 * @brief Removes all elements from the interactable manager
 * NOTE: Not Thread-safe!
 */
void InteractableManager::clear() {
    //Loops through each interactable freeing each element
    for (Interactable* i : interactables) {
        delete i;
    }
    //Then clears the list since they have all been deleted
    interactables.clear();
}


/**
 * @brief Gets an interactable by its ID
 *
 * @param ID The ID of the interactable to get
 * @return const Interactable* The const interactable, nullptr if not found
 */
const Interactable* InteractableManager::getInteractableByID(int ID) const {
    //Return the Mutable version as const
    return getInteractableByIDMut(ID);
}


/*
* Updates all the interactables
* 
* @return int, 0 on success, otherwise the value returned is how many interactables failed to update
*/
int InteractableManager::updateInteractables() {
    int failures = 0;
    for (Interactable* i : interactables) {
        failures += i->update() != 0;
    }
    return 0;
}

/**
 * @brief Gets an interactable mutably by its ID
 *
 * @param ID The ID of the interactable to get
 * @return const Interactable* The mutable interactable, nullptr if not found
 */
Interactable* InteractableManager::getInteractableByIDMut(int ID) const {
    Interactable* foundInteract = nullptr;

    //Loops through each interactable until it finds one with the same ID
    for (Interactable* i : interactables) {
        //If the IDs match
        if (i->getID() == ID) {
            foundInteract = i;
            break;
        }
    }

    //Returns the found interactable
    return foundInteract;
}



/*
* Attempts to click on the interactables
* 
* @param clickX, the click's X position
* @param clickY, the click's Y position
* @return int, 0 on success otherwise an error occured
*/
int InteractableManager::click(int clickX, int clickY) {
    int worked = 0;

    //Clicks on every interactable
    for (Interactable* i : interactables) {
        int worked = i->click(clickX, clickY);
        //If there was an error it exits
        if (worked) break;
    }

    return worked;
}

/*
* Attemps to interact with the manager when the mouse is down
*
* @param downX, The Mouse Down's X position
* @param downY, The Mouse Down's Y position
* @return int, 0 on success otherwise an error occured
*/
int InteractableManager::mouseDown(int downX, int downY) {
    int worked = 0;
    //Mouse's Down on every interactable
    for (Interactable* i : interactables) {
        worked = i->mouseDown(downX, downY);
        //If there was an error it exits
        if (worked) break;
    }

    return worked;
}

/*
* Performs the mouse scroll action on the contained interactables
*
* @param scrollX, The Mouse Scroll's X position
* @param scrollY, The Mouse Scroll's Y position
* @param scrollSpd, The Mouse Scroll's Speed
* @return int, 0 on success, otherwise an error occured
*/
int InteractableManager::mouseScroll(int scrollX, int scrollY, float scrollSpd) {
    int worked = 0;
    //Mouse's Down on every interactable
    for (Interactable* i : interactables) {
        worked = i->mouseScroll(scrollX, scrollY, scrollSpd);
        //If there was an error it exits
        if (worked) break;
    }

    return worked;
}

/**
 * Renders all the interactables in the Manager
 */
void InteractableManager::render() {
    //Loops through each interactable rendering each one
    for (Interactable* i : interactables) {
        i->render();
    }
}
