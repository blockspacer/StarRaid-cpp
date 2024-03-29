/**
 * @addtogroup client
 * @{
 *
 * @file   gfxImage.hpp
 * @author xuedi (xuedi@beijingcode.org)
 * @date   2015-05-29
 * @brief  image control
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
 *    2005-2017 by Daniel (xuedi) Koch
 *
 */

#ifndef gfxImage_H
#define gfxImage_H

#include <string.h>     // strcpy,strcmp
#include <iostream>
#include <sstream>
#include <map>

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_rotozoom.h"

#include "sdlHelper.hpp"



using namespace std;

/// Toolset to handle images
class gfxImage : public sdlHelper {


    public:

//        struct mySurface {
//            int handle;
//        }

        /// Construcor
        gfxImage();

        /// Destrucor
        virtual ~gfxImage();

        /// Load a bitmap into the imae surface plus adjustments
        void load(string pPath);

        /// Load a picture via Load() and then creates a bunch of rotated images
        void loadMultiple(string pPath, int numberRotated);

        /// Is creating a surface by 'hand' and adds it to the surfacepool
        void create(int x, int y);

        /// Copy some surface data into system vars and prepare some more
        void update(void);

        /// Is setting a dafault position
        void setDefaultPositions(int x, int y);

        /// Simpla draw on the default position
        void draw(SDL_Surface *target);

        /// Draw the first image of the pool
        void draw(SDL_Surface *target, Sint16 x, Sint16 y);

        /// Draw a specific image out of the pool
        void draw(SDL_Surface *target, Sint16 x, Sint16 y, int surfaceNumber);

        /// Draw a an image from the center
        void drawCentered(SDL_Surface *target, int x, int y, int surfaceNumber);

        //void deleteSurface(void);

        /// This method returns the selected image from the pool
        SDL_Surface* getSurface(int pNumber);

        /// This method returns the selected image from the pool
        SDL_Surface* getSurface(void);

        /// This is the surface pool
        map<int,SDL_Surface*> surfacePool;

        /// Preproccesed value, holds the width
        int width;

        /// Preproccesed value, holds the height
        int height;

        /// Preproccesed value, holds the half width
        int widthHalf;

        /// Preproccesed value, holds the half height
        int heightHalf;

        /// The default X positions of the image
        int defaultX;

        /// The default Y positions of the image
        int defaultY;


    protected:


    private:


        typedef std::pair<int, SDL_Surface*> surfacePoolPair;
        typedef std::map<int, SDL_Surface*> surfacePoolMap;
        typedef std::map<int, SDL_Surface*>::iterator surfacePoolIterator;


       /// Is creating a surface by 'hand'
        SDL_Surface* createSurface(int x, int y);

        /// If the grafics are inicilised this flag will be set
        bool flagInit;

        /// The name
        string name;


};

#endif // gfxImage_H
/** @} group*/