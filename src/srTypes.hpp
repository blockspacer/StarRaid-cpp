/**
 * @addtogroup global
 * @{
 *
 * @file   srGlobalTypes.h
 * @author xuedi (xuedi@beijingcode.org)
 * @date   2010-11-30
 * @brief  Some Types and definitions that are used everywhere
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

#ifndef SRTYPES_H
#define SRTYPES_H


/**
 * Details: This is a global class that should be used as parent from all classes that
 * share information (types) about the network status and so on
 * @brief Holds global type information
 */
class srTypes {


    public:
        srTypes();
        virtual ~srTypes();

        /// The enum of all the network message types
        enum netMessageType {
          PING,                 //!< will request a PONG response
          PONG,                 //!< answer to the PING request
          VERSION_ASK,          //!< ask for the version
          VERSION_ANSWER,       //!< answer the version number
          LOGIN_ASK,            //!< ask for login
          LOGIN_ANSWER,         //!< answer with login details (name, pass)
          OBJ_BEACON,           //!< has basic object infos, like pos, type, name, id
          OBJ_SELF,             //!< same as beacon but for the object to itself
          CMD_MOVE,             //!< tells where a move_to click was pointed to
          DETAILS_ASK,          //!< ask for more details about an object
          DETAILS_ANSWER,       //!< gives more details about an object depending how much the requester is allowed to see
          TERMINATE,            //!< ask the client to terminate itself
          CHAT,                 //!< a chat message send my client, server adds the sender info and send to others back
          OBJ_TYPE,             //!< send a objectType tp the client
          ACTION                //!< ACTION: a basic shot (server will answer with details of involved objects)
        }; // if new is added, DONT forget the constructor and add the char translation to it

        /// resolves the netMessageType into a char*
        const char* netMessageTypeString[200];

};

#endif // SRGLOBALTYPES_H

/** @} global Group*/
