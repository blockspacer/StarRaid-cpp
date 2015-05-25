#include "server.hpp"
#include <signal.h>		/* CONTROL+C */

// signal handeling
sig_atomic_t run = 1;
void handler( int ) {
    run = 0;
}

int main() {

    // parameter parse parameter (verbose aso) --> getopt.h
    int verbose = 1; // 0:nothing, 1:normal, 2:debug

	// engine
    server starraid(verbose);

    // signal hooks
    signal( SIGINT, &handler );

    // well, loop ^_^
    while ( run ) {
    	starraid.tick();
    }

    // done here
    return 0;
}
