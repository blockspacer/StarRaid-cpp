/**
 * @addtogroup global
 * @{
 *
 * @file   timer.hpp
 * @author xuedi (xuedi@beijingcode.org)
 * @date   2010-10-26
 * @brief  Doing some timing stuff based on SDL
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
 *    2005-2010 by Daniel Koch (xuedi)
 *
 */

#ifndef SR_TIMER_H
#define SR_TIMER_H


#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <vector>


#include "SDL/SDL.h"

using namespace std;

/**
 * Details: There are unlimited number of interupts
 * on request
 * @brief This class creates some timer interupts via SDL
 * @todo The timers get unexact at very fast execution time 1/100 sec.
 */
class srTimer {


    public:
        srTimer();              //!< Constructor
        virtual ~srTimer();     //!< Destructor

        /// Will add a timer
        void addTimer(string timerHandle, int timerIntervalMS);

        /// Checks if timer is overdate
        bool checkTimer(string timerHandle);

        /// Just generates stats
        void tickSecond(void);

        /// gets the last timer infos
        string getTimerInfos(void);


    protected:


    private:

        /// This is the timer entry for the pool
        struct structTimer {
            Uint32 setTime;     //!< The actualy time
            int IntervalMS;     //!< The wake up time for the timer to reset
            string handle;      //!< The handle of the timer
            long stats;         //!< the runtime stats
            int statsSmooth;    //!< the smothen stats, generated on call
        };

        /// The pool of the timers
        vector<structTimer> timers;


};

#endif // TIMER_H

/** @} global Group*/
