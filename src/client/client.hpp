/**
 * @addtogroup client
 * @{
 *
 * @file   client.hpp
 * @author xuedi (xuedi@beijingcode.org)
 * @date   2015-05-29
 * @brief  The client main engine
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

#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>     // strcpy,strcmp
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <time.h>
#include <cstdio>
#include "SDL/SDL.h"

#include "MessageIdentifiers.h"
#include "RakPeerInterface.h"
#include "RakNetTypes.h"
#include "BitStream.h"

#include "../srConfig.hpp"
#include "../srTypes.hpp"
#include "../srObject.hpp"

#include "gfxImage.hpp"
#include "gfxSystem.hpp"


using namespace std;

/// Main engine that runns the Client
class client {

	public:

		/// Constructor
		client();

		/// Destructor
		~client();

		/// Starts up the engine
		void init(string pPath);

		/// Starts up the engine
		void initGFX(void);

		/// Start the gameloop
		bool run(void);

		/// Does the main game logic
		void tick(void);

		/// Calculates runtimedata
		void tickRuntime(void);

		/// Check on menue hovering and click
		void tickMenue(void);

		/// Draw the screen
		void draw(void);

		/// Draw the screen
		void drawGfx(void);

 
	private:

		srConfig config;
		gfxSystem gfx;


		/// The main games surface buffer for blitting
		SDL_Surface *screen;


		/// varion stuff
		int menueHover; //!< If a menue is hoverd
		int version; //!< If a menue is hoverd


		/// Rerender Flag
		bool  render;


		/// Settings
		int   screenResX,
			  screenResY,
			  screenResHalfX,
			  screenResHalfY,
			  screenFull,
			  screenDepth;


		/// Runtime
		int   runtimeFlops,
			  runtimeSleep,
			  runtimeRenders,
			  runtimeFpsSmooth,
			  runtimeRenderSmooth,
			  runtimeOptimumFPS;
		long  runtimeTime,
			  runtimeCount;
		float runtimeDelta;


		/// Objects
		void objectsDraw(void);
		//->structPosition


		/// Network
		void netTick(void);
		void netRead(RakNet::Packet *packet);
		void netSend(int messageType);
		void netReadBeacon(RakNet::Packet *packet, bool flagSelf);
		bool netConnected;
		RakNet::RakPeerInterface* rakPeer;
		RakNet::Packet *rakPacket;


		/// Vector that holds the star types and positions
		struct structStar {
			float x, y;
			int typ;
		};
		vector<structStar> stars;
		int starsCount; //!< Is just counting the Stars
		void starsInit(void);
		void starsMove(int moveX, int moveY);
		void starsDraw();


		/// Objects
		long selfHandle,selfX,selfY;
		map<long, srObject> objects;


		/// Mouse
		int   mouseX,         //!< The actual mouse position X
			  mouseY,         //!< The actual mouse position Y
			  mouseZ,         //!< The actual mouse wheel position
			  memMouseX,      //!< The last mouse position X
			  memMouseY,      //!< The last mouse position Y
			  memMouseDownX,  //!< The last mouse position X before mouse down event
			  memMouseDownY,  //!< The last mouse position Y before mouse down event
			  mouseButton;    //!< Is not 0 if a mouse button is pressed

		long  moveX, 
			  moveY;


		/// is set if the mouse position is changes
		bool  flagMausMove;

		struct structPosition {
			long x, y;
		};

};

#endif // CLIENT_H

/** @} group*/