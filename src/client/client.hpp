#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>     // strcpy,strcmp
#include <iostream>
#include <sstream>
#include <fstream>
#include <time.h>
#include <cstdio>
#include "SDL/SDL.h"

#include "../srTypes.hpp"
#include "../srConfig.hpp"

#include "gfxImage.hpp"
#include "gfxSystem.hpp"

using namespace std;

/// Main engine that runns the Client
class client : public srTypes {

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

        /// Draw the screen
        void draw(void);

 
    private:

    	srConfig config;
        gfxSystem gfx;

        /// The main games surface buffer for blitting
        SDL_Surface *screen;

        /// Rerender Flag
        bool  render;

        /// Settings
        int   screenResX,
              screenResY,
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

};

#endif // CLIENT_H

/** @} Client Group*/