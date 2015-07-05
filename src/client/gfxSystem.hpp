/**
 * @addtogroup client
 * @{
 *
 * @file   gfxSystem.hpp
 * @author xuedi (xuedi@beijingcode.org)
 * @date   2015-05-29
 * @brief  Holds all media as singelton
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

#ifndef GFXSYSTEM_H
#define GFXSYSTEM_H

#include <string.h>     // strcpy,strcmp
#include <iostream>

#include "SDL/SDL.h"
//#include "glSDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_rotozoom.h"
//#include "SDL/SDL_gfx.h"


//#include "config.h"
#include "gfxImage.hpp"
#include "gfxFont.hpp"
#include "sdlHelper.hpp"
//#include "sysConfig.hpp"
//#include "logSystem.h"


class config;

using namespace std;

/// Global singelton that holds all the grafics and media
class gfxSystem : public sdlHelper {

	/// define the singelton pointer
	static gfxSystem *globInstance;

	public:

		/// This class is used as an global singeltin instance
		static gfxSystem *instance() {
			if (!globInstance) globInstance = new gfxSystem;
			return globInstance;
		}






		/// Construcor
		gfxSystem();

		/// Destrucor
		virtual ~gfxSystem();

		/// Sets a few internal options
		void init(int resX, int resY);


		// Main ressources
		gfxImage *sfNone;
		gfxImage *sfMenue;
		gfxImage *sfMsg;
//		gfxImage *sfRadarSpotNeutralSmall;
//		gfxImage *sfMouseSpot;
//		gfxImage *sfStarBig, *sfStarMiddle, *sfStarSmall;


		/// Contains a Arial12 font
		gfxFont *fontArial12;

		/// Contains a Arial12 font
		gfxFont *fontArial18;

		/// Draw a surface out of the pool
		void poolAdd(string pName, string pFileName);

		/// Draw a surface out of the pool
		void poolAdd(string pName, string pFileName, int pRotate);

		/// puzzels together a menue
		void createMenue(int resX, int resY, int hover);

		/// request a surface
		gfxImage* poolGet(string pKey);


	protected:
	private:

		// type to make the code more clean
		typedef std::pair<string, gfxImage*> gfxPoolPair;               //!< The Pair for the gfxPool
		typedef std::map<string, gfxImage*> gfxPoolMap;                 //!< The Map for the gfxPool
		typedef std::map<string, gfxImage*>::iterator gfxPoolIterator;  //!< The Iterator for the gfxPool

		/// This is the gfxPool
		map<string,gfxImage*> gfxPool;

		/// Will be set after the load proccess is done
		bool flagLoaded;

		/// Does the decorations for the chat window
		void createMsgWindow(int resX, int resY, int msgWinW, int msgWinH);



};

#endif // GFXSYSTEM_H
/** @} group*/