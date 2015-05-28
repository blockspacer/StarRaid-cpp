#include "sdlHelper.hpp"

sdlHelper::sdlHelper() {
    //ctor
}

sdlHelper::~sdlHelper() {
    //dtor
}


/**
 * Make an SDL_Rect without manually setting each value one at a time
 * &(SDL_Rect){0,0,0,0} would be possible as well
 *   @note this function as well as plain &(SDL_Rect){0,0,0,0} always get a compiler warning, so better create an object
 *   @param xs Source x position
 *   @param ys Source y position
 *   @param dx Destination x position
 *   @param dy Destination y position
 */
SDL_Rect sdlHelper::newSDL_Rect( int xs,int ys, int dx,int dy ) {
    SDL_Rect rc;
        rc.x = xs; rc.y = ys;
        rc.w = dx; rc.h = dy;
    return( rc );
}

/**
 * Make a new SDL_Color with alpha
 *   @param r Red
 *   @param g Green
 *   @param b Blue
 *   @param a Alpha
 */
SDL_Color sdlHelper::newSDL_Color( int r, int g, int b, int a ) {
    SDL_Color clr;
        clr.r = r;
        clr.g = g;
        clr.b = b;
        clr.unused = a;
    return( clr );
}

/**
 * Make a new SDL_Color
 *   @param r Red
 *   @param g Green
 *   @param b Blue
 */
SDL_Color sdlHelper::newSDL_Color( int r, int g, int b ) {
    SDL_Color clr;
        clr.r = r;
        clr.g = g;
        clr.b = b;
        clr.unused = 0;
    return( clr );
}



/*
    // operators are fun
    bool operator==(SDL_Color a, SDL_Color b) {
    	return( a.r == b.r && a.g == b.g && a.b == b.b );
    }
    bool operator!=(SDL_Color a, SDL_Color b) {
    	return( ( a.r != b.r || a.g != b.g || a.b != b.b ) );
    }
    // It's faster to type this way
    int SDL_MapRGB( SDL_PixelFormat *format, SDL_Color clr ) {
    	return( SDL_MapRGB( format, clr.r, clr.g, clr.b ) );
    }
    // Same here
    int SDL_FillRect( SDL_Surface *dest, SDL_Rect *rc, SDL_Color &clr ) {
    	if( dest == NULL ) {  return(-1);  }
    	return( SDL_FillRect( dest, rc, SDL_MapRGB( dest->format, clr ) ) );
    }

*/
