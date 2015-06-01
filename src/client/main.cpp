/**
 * @addtogroup client
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
 *    2005-2015 by Daniel (xuedi) Koch
 *
 */

#include "client.hpp"
#include "gfxSystem.hpp"

gfxSystem *gfxSystem::globInstance = 0;


using namespace std;

int main(int argc, char *argv[]) {

    /// create the game engine
    client myClient;

    /// init the engine
    myClient.init("client.xml");

    int end=0;
    while(end==0)  {

        /// run the game
        end = myClient.run();   // reboot on 0 end on 1
    }

    return 0;
}
/** @} group*/