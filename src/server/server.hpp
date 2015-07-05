/**
 * @addtogroup server
 * @{
 *
 * @file   server.hpp
 * @author xuedi (xuedi@beijingcode.org)
 * @date   2015-05-29
 * @brief  The server main engine
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

#ifndef SERVER_H
#define SERVER_H


#include <string.h>			// strcpy,strcmp
#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <vector>
#include <unistd.h> 		//usleep

#include "MessageIdentifiers.h"
#include "RakPeerInterface.h"
#include "RakNetTypes.h"
#include "BitStream.h"

#include "database.hpp"
#include "networkClient.hpp"

#include "../srTimer.hpp"
#include "../srConfig.hpp"
#include "../srTypes.hpp"


using namespace std;

class server {

	private:
		long runtime;	//!< In var count up every second and contains the runtime
		int fps,        //!< This var counts the frames together and is reset every second
			fpsSmooth;  //!< The secondly result of fps is bufferd here
		int lps,        //!< This var counts calculation loop
			lpsSmooth;  //!< The secondly result of lps is bufferd here
		int blps,		//!< Count the big loop per second
			blpsSmooth;	//!< The secondly result of blps is bufferd here
		int dps,        //!< Counts the network deliveries
			dpsSmooth;  //!< Is the smothen networkdelivery per second
		int sysBreak,	//!< This is a very important var, it adjust the amount of time giving back to the operating system
			sysNoise;
		int calcDist;	//!< how far the calculation should grep (CPU)

	   srTimer timers;
	   database db;

	public:
		server(string pPath);
		~server();

		//### main logic
		void tick(void);

		//### objects
		void objLoad(void);
		void objLoop(void);
		map<long, srObject> objects;

		//### network
		void netInit(void);
		void netTick(void);
		void netRead(RakNet::Packet *packet);
		void netSend(int messageType, RakNet::SystemAddress adress);
		void netClientCheck(void);
		void netClientAdd(RakNet::Packet *packet);
		void netClientTerminate(RakNet::SystemAddress address);
		int netTx,	//!< counts the send per second
			netRx;	//!< counts the recive per second
		RakNet::RakPeerInterface* rakPeer;
		RakNet::Packet *rakPacket;
		map<RakNet::SystemAddress, networkClient> clients;

		//### other stuff
		ofstream logfile;
		srConfig config;
};

#endif // SERVER_H
/** @} group*/