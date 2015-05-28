#include "gfxFont.hpp"

gfxFont::gfxFont() {
//    colorFront = NULL;;
//    colorBackground = NULL;;
    flagInit=0;

}

gfxFont::~gfxFont() {
    //dtor
}


void gfxFont::load(string pPath, int size, int red, int green, int blue, int alpha) {

    font = TTF_OpenFont(pPath.c_str(), size);
    if(font == NULL){
        cout << "gfxFont::load: " << TTF_GetError() << endl;
        exit(1);
    }

    // TODO-1: check parameter
    colorFront = newSDL_Color(red, green, blue, alpha);

    flagInit=1;

}

/**
 * The font function also draw a text out
 *   @todo To be replace through dynamic system like printf
 *   @param target The system surface into wich is will draw
 *   @param x The X position
 *   @param y The Y position
 *   @param text the actual text the will be used
 */
void gfxFont::drawtext(SDL_Surface *target, int x, int y, string text) {

    SDL_Rect tmpRect;

    if(flagInit==1) {
        SDL_Surface *resulting_text;
        resulting_text = TTF_RenderText_Solid(font, text.c_str(), colorFront);
        tmpRect = newSDL_Rect(x,y,0,0);
        SDL_BlitSurface(resulting_text , NULL, target, &tmpRect );
    } else {
        cout << "gfxFont::drawtext: The choosen font is not loaded jet! " << endl;
        exit(1);
    }
}


/**
 * Make an SDL_Rect without manually setting each value one at a time
 * &(SDL_Rect){0,0,0,0} would be possible as well
 *   @todo To be replaced by new drawtext()
 *   @param target The system surface into wich is will draw
 *   @param x The X position
 *   @param y The Y position
 *   @param text the actual text the will be used
 *   @param pair And additional value that will be added
 */
void gfxFont::drawtextPair(SDL_Surface *target, int x, int y, string text, int pair) {

    std::string s;
    std::stringstream out;
    out << text << pair;
    s = out.str();

    drawtext(target, x, y, s);

}

/**
 * Make an SDL_Rect without manually setting each value one at a time
 * &(SDL_Rect){0,0,0,0} would be possible as well
 *   @todo To be replaced by new drawtext()
 *   @param target The system surface into wich is will draw
 *   @param x The X position
 *   @param y The Y position
 *   @param text the actual text the will be used
 *   @param pair And additional value that will be added
 */
void gfxFont::drawtextPair(SDL_Surface *target, int x, int y, string text, float pair) {

    std::string s;
    std::stringstream out;
    out << text << pair;
    s = out.str();

    drawtext(target, x, y, s);

}

/**
 * Make an SDL_Rect without manually setting each value one at a time
 * &(SDL_Rect){0,0,0,0} would be possible as well
 *   @todo To be replaced by new drawtext()
 *   @param target The system surface into wich is will draw
 *   @param x The X position
 *   @param y The Y position
 *   @param text the actual text the will be used
 *   @param pair And additional value that will be added
 */
void gfxFont::drawtextPair(SDL_Surface *target, int x, int y, string text, long pair) {

    std::string s;
    std::stringstream out;
    out << text << pair;
    s = out.str();

    drawtext(target, x, y, s);

}

/**
 * Make an SDL_Rect without manually setting each value one at a time
 * &(SDL_Rect){0,0,0,0} would be possible as well
 *   @todo To be replaced by new drawtext()
 *   @param target The system surface into wich is will draw
 *   @param x The X position
 *   @param y The Y position
 *   @param text the actual text the will be used
 *   @param pair And additional value that will be added
 */
void gfxFont::drawtextPair(SDL_Surface *target, int x, int y, string text, string pair) {

    std::string s;
    std::stringstream out;
    out << text << pair;
    s = out.str();

    drawtext(target, x, y, s);
}
