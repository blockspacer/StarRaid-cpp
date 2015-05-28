/**
 * @addtogroup global
 * @{
 *
 * @file   sdlHelper.h
 * @author xuedi (xuedi@beijingcode.org)
 * @date   2010-10-26
 * @brief  Some functions typical for SDL but not in the SDL code (SDL_BOOST^^)
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
 *    2005-2010 by Daniel Koch (xuedi)
 *
 */

#ifndef SDLHELPER_H
#define SDLHELPER_H


#include "SDL/SDL.h"

/// sdlHelper functions (newSDL_Rect() and so on)
class sdlHelper {


    public:

        /// Construcor
        sdlHelper();

        /// Destrucor
        virtual ~sdlHelper();

        /// This function returns a SDL_Rect
        SDL_Rect newSDL_Rect( int xs,int ys, int dx,int dy );

        /// This function returns a SDL_Color
        SDL_Color newSDL_Color( int r, int g, int b, int a );

        /// Is similar to newSDL_Color just without alpha
        SDL_Color newSDL_Color( int r, int g, int b );

    protected:



    private:



};

#endif // SDLHELPER_H

/** @} global Group*/
