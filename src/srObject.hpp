/**
 * @addtogroup shared
 * @{
 *
 * @file   srObject.h
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


        /// the unique handle to identify the object
        long handle;

        /// the type of the object
        int type;

        /// the status of the object (destroyes, active and so on)
        int status;

        /// The name of the object
        string name;


        long x, //!< Its position on the x axis
             y; //!< Its position on the y axis

        /// the character that is controlling this object (should be moved to server)
        int character_id;

        //////////////////////// BASE Attributes ////////////////////////
        int shield,     //!< Shield
            health,     //!< Health
            scanner,    //!< Scanner
            jammer;     //!< Jammer

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

/** @} global Group*/
