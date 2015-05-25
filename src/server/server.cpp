#include "server.hpp"

using namespace std;

server::server(int verbose) {
    sysNoise = verbose;
    cnt = 0;

    // timers
    fps = 0;
    lps = 0;
    dps = 0;
    fpsSmooth = 0;
    lpsSmooth = 0;
    dpsSmooth = 0;
    timers.addTimer("second", 1000); //   1 times per second
    timers.addTimer("tenth", 100);  //  10 times per second
    timers.addTimer("100th", 10);   // 100 times per second
}

server::~server() {
    // disconnect and end gracefully
}

void server::tick(void) {
    fps++;

    // second
    if( timers.checkTimer("second") ) {
        fpsSmooth = fps; // ticks per second
        lpsSmooth = lps; // loops per second
        dpsSmooth = dps; // network packages per second
        timers.tickSecond();
        runtime++;

        fps = 0;
        lps = 0;
        dps = 0;

        if(sysNoise>=1) {
            cout << "Tick: " << fpsSmooth;
            //cout << " | " << timers.getTimerInfos();
            cout << endl;
        }
    }

    // 10x second
    if( timers.checkTimer("tenth") ) {
        //
    }

    // 100x second
    if( timers.checkTimer("100th") ) {
        //
    }


    // sleep a micro second, about 10000 ticks/second
    usleep(1);
}



