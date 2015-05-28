#include <string.h>     // strcpy,strcmp
#include <iostream>
#include <sstream>
#include <fstream>
#include <unistd.h> //usleep

#include "../srTimer.hpp"
#include "../srTypes.hpp"
#include "../srConfig.hpp"

class server : public srTypes {

	private:
        long runtime;	//!< In var count up every second and contains the runtime
        int fps,        //!< This var counts the frames together and is reset every second
            fpsSmooth;  //!< The secondly result of fps is bufferd here
        int lps,        //!< This var counts calculation loop
            lpsSmooth;  //!< The secondly result of lps is bufferd here
        int dps,        //!< Counts the network deliveries
            dpsSmooth;  //!< Is the smothen networkdelivery per second
        int sysBreak,	//!< This is a very important var, it adjust the amount of time giving back to the operating system
        	sysNoise;

	srTimer timers;

    public:
        server(string pPath);
        ~server();
        void tick(void);
        int cnt;

        ofstream logfile;
        srConfig config;
};