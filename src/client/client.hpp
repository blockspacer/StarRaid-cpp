#ifndef CLIENT_H
#define CLIENT_H

#include <string.h>     // strcpy,strcmp
#include <iostream>
#include <sstream>
#include <fstream>
#include <time.h>       // usleep
#include <time.h>       // usleep
#include <cstdio>
#include "SDL/SDL.h"

#include "../srTypes.hpp"
#include "../srConfig.hpp"


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

        /// Start the gameloop
        void stop(void);

        /// Draw the screen
        void draw(void);

 
    private:

    	ofstream logfile;
    	srConfig config;

        /// The main games surface buffer for blitting
        SDL_Surface *screen; 

};

#endif // CLIENT_H

/** @} Client Group*/
