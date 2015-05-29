/**
 * @addtogroup shared
 * @{
 *
 * @file   srConfig.h
 * @author xuedi (xuedi@beijingcode.org)
 * @date   2015-05-29
 * @brief  This file contains the config cache
 * @todo add common elements into a common base class and its values into the config files
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

#ifndef CONFIG_H
#define CONFIG_H


#include <string.h>     // strcpy,strcmp
#include <iostream>     // std::cout
#include <fstream>      // std::ifstream
#include <map>          // key/value store
#include <stdio.h>      // atoi
#include <stdlib.h>     // atoi


//#include <yaml.h> //  -lyaml




using namespace std;

/// This file contains the config cache
class srConfig {

    public:

        /// Constructor
        srConfig();

        /// Destructor
        ~srConfig();

        /// init the config system
        void init(bool flagDebuging);

        /// Load the config file
        void load(string pPath);

        /// This is the actual key/value config pool
        map<string, string> cache;

        /// graps an interger value out of the pool
        int getValueInt(string Key, int defaultValue);

        /// graps a string out of the pool
        string getValueString(string Key, string defaultValue);

};

#endif // CONFIG_H
/** @} group*/