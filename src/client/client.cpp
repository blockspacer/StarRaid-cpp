#include "client.hpp"

client::client() {
    //
}


client::~client() {
    //
}


void client::init(string pPath) {
    logfile.open("client.log", ios::out);
    logfile << "---- starting client ----" << endl;

    config.load(pPath);

    logfile << "OK" << endl;
}


void client::stop(void) {
    logfile.close();
}


void client::initGFX(void) {
    logfile << "---- starting GFX ----" << endl;

    int resX = config.getValueInt("ResolutionX", 640);
    int resY = config.getValueInt("ResolutionY", 480);
    int full = config.getValueInt("ResolutionFullscreen", 0);
    int depth = config.getValueInt("ResolutionDepth", 32);

    // set SDL exit routines
    atexit(SDL_Quit);

    // init the SDL library
    if(SDL_Init(SDL_INIT_VIDEO) < 0 ) {
        logfile << "engine::initSDL: Couldn't initialize SDL: " << SDL_GetError() << endl;
        exit(1);
    }

    // set main screen
    if(full) screen = SDL_SetVideoMode(resX, resY, depth, SDL_DOUBLEBUF | SDL_HWSURFACE | SDL_FULLSCREEN);
    else screen = SDL_SetVideoMode(resX, resY, depth, SDL_DOUBLEBUF | SDL_HWSURFACE);
    if (screen == NULL) {
        logfile << "engine::initSDL: Couldn't set Resolution (" << resX << "x" << resY << "):" << SDL_GetError() << endl;
        exit(1);
    }

    // inicialize the network engine
    logfile << "OK" << endl;
}


bool client::run(void) {

    // init SDL
    initGFX();

    // game loop
	while(1) {

        SDL_Event event;
        if(SDL_PollEvent(&event)) {

            switch( event.type ) {
                case SDL_KEYDOWN:

                    switch(event.key.keysym.sym) {

                        case SDLK_ESCAPE:
                            return true; // EXIT the game
                        break;

                        // default just because of compiler warning
                        default:
                        break;

                    }

                break;

            }
        }

	}
}



void client::draw(void) {
    //
}

