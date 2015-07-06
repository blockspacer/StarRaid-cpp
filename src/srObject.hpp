/**
 * @addtogroup shared
 * @{
 *
 * @file   srObject.hpp
 * @author xuedi (xuedi@beijingcode.org)
 * @date   2015-05-29
 * @brief  The main object components that server AND client have to have
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

#ifndef SROBJECT_H
#define SROBJECT_H


#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>

#include "MessageIdentifiers.h"
#include "RakPeerInterface.h"
#include "RakNetTypes.h"
#include "BitStream.h"

#include "srCargo.hpp"

using namespace std;

/**
 * This hold the most important attibutes that are used by the server and client to
 * work with the object
 * @warning Do not Change or modify objBASE, this are core elements of the Server&Cliend classes
 */
class srObject {


	public:
		srObject();
		virtual ~srObject();

		//### runtime ###
		long queCnt;		//!< Add every big loop, until item is processed
		int queCntBig;		//!> The number of time this item was proccessed (for big one)
		int queCntBigMax;	//!> The number of time this item was proccessed (for big one)

		/// the game relevant game neighbours around the object
		vector<long> neighbours;
		RakNet::SystemAddress pilotAddress;

		//### basics ###
		long handle;	//!< The unique handle to identify the object
		int type;		//!< The type of the object (main class)
		int status;		//!< The status of the object (destroyes, active and so on)
		int owner;		//!< The controlling player/npc
		string name;	//!< The name of the object
		
		long x, //!< Its position on the x axis
			 y; //!< Its position on the y axis


		//### stats ###
		int shield,		//!< Shield
			health,		//!< Health
			scanner,	//!< Scanner
			jammer;		//!< Jammer

		/// add cargo
		void cargoAdd(string pType, int pAmount);

		/// get cargo
		int cargoGet(string pType);

	private:

		/// cargo pool
		vector<srCargo> cargo;
		vector<srCargo>::iterator cargoIterator;


};

#endif // SROBJECT_H
/** @} group*/
