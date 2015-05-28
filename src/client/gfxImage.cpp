#include "gfxImage.hpp"

/**
 * Set inicial values to NULL
 */
gfxImage::gfxImage() {
//    surface = NULL;
    flagInit = 0;
    defaultX=0;
    defaultY=0;
}

/**
 * Mainly destruct the surface pool and free the SDL images
 */
gfxImage::~gfxImage() {

    // free all surfaces
    surfacePoolIterator i = surfacePool.begin();
    for(;i != surfacePool.end();++i) SDL_FreeSurface(i->second);
    surfacePool.clear();

}

/**
 * Log description
 *   @param pNumber The number of the imsge from the surfacepool to return
 */
SDL_Surface* gfxImage::getSurface(int pNumber) {
    surfacePoolIterator i = surfacePool.find(pNumber);
    if(i == surfacePool.end()) {
        cout << "gfxImage::getSurface: Could not get surfacePool: " << pNumber << endl;
        exit(1);
    }
    return i->second;
}

/**
 * Long description
 *   @param pPath The location of the image to load
 *   @param numberRotated The number of rotated surfaces to create from first
 */
void gfxImage::loadMultiple(string pPath, int numberRotated) {


    // load main surfacePool
    load(pPath);

    // add rotated surfaces
    int i;
    SDL_Rect tmpRectRotated, tmpRectCreated;
    float rotateSteps = 360 / numberRotated; // minus one because 360 is also zero
//    rotateSteps = 7.5;
    SDL_Surface *tmpRotatedSurface, *tmpCreatedSurface;
    for(i=1;i<=numberRotated;i++) {

        // get the temp-rotated surface
        tmpRotatedSurface = rotozoomSurface(getSurface(0),i*rotateSteps,1,0);

        // create the result-surface
        tmpCreatedSurface = createSurface(getSurface(0)->w, getSurface(0)->h);

        // fill created surface with magic pink
        SDL_FillRect(tmpCreatedSurface, NULL, SDL_MapRGB( tmpCreatedSurface->format, 255,0,255 ));

        // because the rotated surface is bigger, i need to copy the half difference to keep the rotation point in the middle
        tmpRectRotated.x = (int)(tmpRotatedSurface->w-tmpCreatedSurface->w)/2;
        tmpRectRotated.y = (int)(tmpRotatedSurface->h-tmpCreatedSurface->h)/2;
        tmpRectRotated.w = getSurface(0)->w;
        tmpRectRotated.h = getSurface(0)->h;

        // the target is the same size as the picture 0
        tmpRectCreated.x = 0;
        tmpRectCreated.y = 0;
        tmpRectCreated.w = getSurface(0)->w;
        tmpRectCreated.h = getSurface(0)->h;

        // blit the rotated surface into the created one
        SDL_BlitSurface(tmpRotatedSurface, &tmpRectRotated, tmpCreatedSurface, &tmpRectCreated );

        // add surface to the surfacepool
        surfacePool.insert(surfacePoolPair(surfacePool.size(), tmpCreatedSurface ) );
    }

    // useless, just to avoid compiling warning because of possible not used
    tmpRotatedSurface = NULL;
    tmpCreatedSurface = NULL;

    SDL_FreeSurface(tmpCreatedSurface);
    SDL_FreeSurface(tmpRotatedSurface);
}




/**
 * Loads a picture from an bitmap source and correct the type according
 * to the game setting, the setup is taken from the main surface image
 *   @param pPath the location of the image to load
 */
void gfxImage::load(string pPath) {

    SDL_Surface *tmpSurfaceA, *tmpSurfaceB;

    // load picture
    tmpSurfaceA = IMG_Load(pPath.c_str());
    if(tmpSurfaceA == NULL) {
        cout << "gfxImage: " << SDL_GetError() << endl;
    }

    // set magic pink
    tmpSurfaceB = SDL_DisplayFormat(tmpSurfaceA);    // prepare the magic pink
    SDL_SetColorKey( tmpSurfaceB, SDL_SRCCOLORKEY|SDL_RLEACCEL, SDL_MapRGB( tmpSurfaceB->format, 255,00,255 ) );


    // add to the surface pool
    surfacePool.insert(surfacePoolPair(surfacePool.size(), tmpSurfaceB));

    // update the imae values
    update();
    flagInit=1;


    // set name of the image
    std::stringstream tmpMessage;
    tmpMessage << "load (" << pPath << ")";
    name = tmpMessage.str();


    // free temp surface
    SDL_FreeSurface(tmpSurfaceA);

    name = pPath;
}



SDL_Surface* gfxImage::createSurface(int x, int y) {

    SDL_Surface*tmpSurfaceA,*tmpSurfaceB;

    /* SDL interprets each pixel as a 32-bit number, so our masks must depend on the endianness (byte order) of the machine */
    Uint32 rmask, gmask, bmask, amask;

    #if SDL_BYTEORDER == SDL_BIG_ENDIAN
        rmask = 0xff000000;
        gmask = 0x00ff0000;
        bmask = 0x0000ff00;
        amask = 0x000000ff;
    #else
        rmask = 0x000000ff;
        gmask = 0x0000ff00;
        bmask = 0x00ff0000;
        amask = 0xff000000;
    #endif
    tmpSurfaceA = SDL_CreateRGBSurface(SDL_SWSURFACE, x, y, 8, rmask, gmask, bmask, amask);
    if(tmpSurfaceA == NULL) {
        cout << "gfxImage: " << SDL_GetError() << endl;
    }

    tmpSurfaceB = SDL_DisplayFormat(tmpSurfaceA);    // prepare the magic pink
    SDL_SetColorKey( tmpSurfaceB, SDL_SRCCOLORKEY|SDL_RLEACCEL, SDL_MapRGB( tmpSurfaceB->format, 255,00,255 ) );

    // color grey
    SDL_FillRect(tmpSurfaceB, NULL, SDL_MapRGB( tmpSurfaceB->format, 150,150,150 ));


    // free temp surface
    SDL_FreeSurface(tmpSurfaceA);


    return tmpSurfaceB;
}



/**
 * Long description
 *   @param x The width of the surface
 *   @param y The heigth of the surface
 */
void gfxImage::create(int x, int y) {


    // add to the surface pool
    surfacePool.insert(surfacePoolPair(surfacePool.size(), createSurface(x, y) ));

    // only the first created surface, if you do LoadMulti its not nessesary
    if(surfacePool.size() <= 1) {
        update();
        flagInit=1;

        // set name of the image
        std::stringstream tmpMessage;
        tmpMessage << "create (" << x << ", " << y << ")";
        name = tmpMessage.str();
    }


}


/**
 * Long description
 */
void gfxImage::update(void) {

    // image itself
    width = (int)getSurface(0)->w;     //TODO-0: use surface.begin() or so
    heigth = (int)getSurface(0)->h;
    widthHalf = (int)width/2;
    heigthHalf = (int)heigth/2;
}


/**
 * If a a draw function will draw an image, and it has default positions
 * set, it wil use them for the drawing
 * @note A minus value start counting then from the other side
 */
void gfxImage::setDefaultPositions(int x, int y) {
    defaultX = x;
    defaultY = y;
}


/**
 * This method will move the image a half image size to the left top to make it centered
 * and then draw it.
 *   @param target The system SDL surface into wich is drawn
 *   @param x The x position
 *   @param y The y position
 *   @param flagDefault If set it will take it own saved positions, usefull for static positions
 *   @param surfaceNumber The number of the image in the pool
 */
void gfxImage::drawCentered(SDL_Surface *target, int x, int y, bool flagDefault, int surfaceNumber) {

    // set default if nothing is set
    if(flagDefault) {
        x = defaultX;
        y = defaultY;
    }
    x -= widthHalf;
    y -= heigthHalf;

    draw(target,x,y,0,surfaceNumber);

}


/**
 * Long description
 *   @param target The system SDL surface into wich is drawn
 *   @param x The x position
 *   @param y The y position
 *   @param flagDefault If set it will take it own saved positions, usefull for static positions
 *   @param surfaceNumber The number of the image in the pool
 */
void gfxImage::draw(SDL_Surface *target, int x, int y, bool flagDefault, int surfaceNumber) {

    if(flagInit==1) {

        // if the wished surface doesn exist give default
//        if(surfaceLoaded<surfaceNumber) surfaceNumber = 0;


        // set default if nothing is set
        if(flagDefault) {
            x = defaultX;
            y = defaultY;
        }

//        SDL_BlitSurface(getSurface(surfaceNumber), NULL, target, &(SDL_Rect){x,y,0,0} );
        SDL_Rect tmpRect = {x,y,0,0};
        SDL_BlitSurface(getSurface(surfaceNumber), NULL, target, &tmpRect );


    } else {
        cout << "gfxImage::draw: The choosen image is not loaded jet! " << endl;
        exit(1);
    }
}


/**
 * Long description
 *   @param target The system SDL surface into wich is drawn
 *   @param x The x position
 *   @param y The y position
 *   @param flagDefault If set it will take it own saved positions, usefull for static positions
 */
void gfxImage::draw(SDL_Surface *target, int x, int y, bool flagDefault) {
    draw(target, x, y, flagDefault, 0);
}


/**
 * Long description
 *   @param target The system SDL surface into wich is drawn
 */
void gfxImage::drawDefault(SDL_Surface *target) {
    draw(target, 0, 0, 1, 0);
}
