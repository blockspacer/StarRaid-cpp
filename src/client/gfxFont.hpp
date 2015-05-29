/**
 * @addtogroup client
 * @{
 *
 * @file   gfxFont.hpp
 * @author xuedi (xuedi@beijingcode.org)
 * @date   2015-05-29
 * @brief  Responsible for fond loading and dawn on surface commands
 *
 * License:
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright:
 *    2005-2015 by Daniel (xuedi) Koch
 *
 */

#ifndef GFXFONT_H
#define GFXFONT_H


#include <string.h>     // strcpy,strcmp
#include <iostream>
#include <sstream>      // drawtextPairInt


#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"


#include "sdlHelper.hpp"



using namespace std;

/// Responsible for fond loading and dawn on surface commands
class gfxFont : public sdlHelper {


    public:

        /// Construcor
        gfxFont();

        /// Destrucor
        virtual ~gfxFont();

        /// This methos loads the font and sets its attibutes
        void load(string pPath, int size, int red, int green, int blue, int alpha);

        /// The basic draw method
        void drawtext(SDL_Surface *target, int x, int y, string text);

        /// Draws text and a int
        void drawtextPair(SDL_Surface *target, int x, int y, string text, int pair);

        /// Draws text and a float
        void drawtextPair(SDL_Surface *target, int x, int y, string text, float pair);

        /// Draws text and a long
        void drawtextPair(SDL_Surface *target, int x, int y, string text, long pair);

        /// Draws text and a long
        void drawtextPair(SDL_Surface *target, int x, int y, string text, string pair);

    protected:


    private:

        /// This variable contains the Fontcolor itself
        SDL_Color colorFront;

        /// This variable contains the Background color
        SDL_Color colorBackground;

        /// This will hold the name of the Font
        string style;

        /// This is the font object by TTF_Font out of the SDl extention
        TTF_Font* font;

        /// Is set if font is inicalised
        bool flagInit;


};

#endif // GFXFONT_H
/** @} group*/
