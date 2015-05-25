#include "srTimer.hpp"

//NOTE: this srTimer class wate quite a little recources, ment if you use msleep(1) or so

srTimer::srTimer() {
	atexit(SDL_Quit);

	// init the SDL library
	if(SDL_Init(SDL_INIT_TIMER) < 0 ) {
		cout << "engine::initSDL: Couldn't initialize SDL_INIT_srTimer: " << SDL_GetError() << endl;
		exit(1);
	}

}


srTimer::~srTimer() {
    SDL_Quit();
}

/**
 * Adds a new timer to the pool
 *   @param srTimerHandle The handle of the timer for later access
 *   @param srTimerIntervalMS The time when the timer will be trigert
 */
void srTimer::addTimer(string srTimerHandle, int srTimerIntervalMS) {
    structTimer temp;
    temp.handle = srTimerHandle;
    temp.stats = 0;
    temp.statsSmooth = 0;
    temp.IntervalMS = srTimerIntervalMS;
    temp.setTime = SDL_GetTicks() + srTimerIntervalMS;
    timers.push_back(temp);
}

/**
 * Does some statistic stuff every second, it will get the time since last call, so better call every second
 */
void srTimer::tickSecond(void) {
    vector<structTimer>::iterator i;
    for( i=timers.begin(); i!=timers.end(); ++i) {
        (*i).statsSmooth = (*i).stats;
        (*i).stats = 0;
    }
}

/**
 * Checks if the timer is over its date, and in case resets the timer
 *   @note Need to be called every frame
 *   @param timerHandle The handle of the timer for later access
 *   @return if over time return true, otherwise false
 */
bool srTimer::checkTimer(string timerHandle) {
    bool retVal=0;
    vector<structTimer>::iterator i;
    for( i=timers.begin(); i!=timers.end(); ++i) {
        if( (*i).handle == timerHandle ) {
            if( SDL_GetTicks() > (*i).setTime ) {
                (*i).setTime = SDL_GetTicks() + (*i).IntervalMS; //  reset srTimer
                (*i).stats++;
                retVal = 1;
            }
        }
    }
    return retVal;
}


/**
 * Returns a string os stats of all its timers
 *   @return Returns a stats string
 */
string srTimer::getTimerInfos(void) {
    std::stringstream retVal;
    vector<structTimer>::iterator i;
    for( i=timers.begin(); i!=timers.end(); ++i) retVal << (*i).handle << ":" << (*i).statsSmooth << " ";
    return retVal.str();
}

