/**
 * @addtogroup server
 * @{
 *
 * @file   main.cpp
 * @author xuedi (xuedi@beijingcode.org)
 * @date   2015-05-29
 * @brief  Just main
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
    server starraid("server.conf");

    // signal hooks
    signal( SIGINT, &handler );

    // well, loop ^_^
    while ( run ) {
    	starraid.tick();
    }

    // done here
    return 0;
}
/** @} group*/