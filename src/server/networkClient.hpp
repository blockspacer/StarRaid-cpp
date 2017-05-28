/**
 * @addtogroup shared
 * @{
 *
 * @file   srNetworkClient.h
 * @author xuedi (xuedi@beijingcode.org)
 * @date   2015-06-01
 * @brief  This file contains the netClient class.
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

#ifndef NETWORKCLIENT_H
#define NETWORKCLIENT_H


#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <utility>

#include "MessageIdentifiers.h"
#include "RakPeerInterface.h"
#include "RakNetTypes.h"
#include "BitStream.h"


using namespace std;

///All the events are handled here, this class is called in the mainloop and processes all the RakNet events via poll
class networkClient {


    public:

        /// Construcor
        networkClient();

        /// Destrucor
        virtual ~networkClient();

        /// This var will count the laag time
        int cntLaag;

        /// this var will count the failed login tries
        int cntFailedLogin;

        /// This var has the general status of the network engine
        int Status;

        /// This var holds the name of the player
        string name;

        /// This var contains the IP address as well as the port number of the connection
        RakNet::SystemAddress address;

        /// This var if not 0 will hold the ID of the refering object that is controlled by the connection
        long objektID;


    protected:


    private:


};

#endif // NETWORKCLIENT_H
/** @} group*/
