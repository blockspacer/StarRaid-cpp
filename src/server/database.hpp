/**
 * @addtogroup server
 * @{
 *
 * @file   database.hpp
 * @author xuedi (xuedi@beijingcode.org)
 * @date   2015-05-29
 * @brief  loads stuff from the database
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

#ifndef DATABASE_H
#define DATABASE_H


#include <string.h>		// strcpy,strcmp
#include <unistd.h> 	// usleep
#include <iostream>
#include <sstream>
#include <vector>
#include <exception>    // trough
#include <mysql/mysql.h>

#include "../srCargo.hpp"
#include "../srObject.hpp"


using namespace std;

class database {

	public:
		database();
		~database();
		void connect(string pName, string pHost, string pUser, string pPass, string pPort);
		long login(string user, string pass);
		vector<srObject> loadObjects(void);

	private:
		string name;
		string host;
		string user;
		string pass;
		string port;
		MYSQL *sqlSocket;
		bool connected;
};

#endif // DATABASE_H
/** @} group*/