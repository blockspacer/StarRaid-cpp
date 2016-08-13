#include "database.hpp"

using namespace std;

database::database() {
	name = "";
	host = "";
	user = "";
	pass = "";
	port = "";
	sqlSocket = NULL;
	connected = false;
}

database::~database() {
	mysql_close(sqlSocket);
}

void database::connect(string pName, string pHost, string pUser, string pPass, string pPort) {
	name = pName;
	host = pHost;
	user = pUser;
	pass = pPass;
	port = pPort;
	sqlSocket = mysql_init(0);
	if(sqlSocket == NULL) {
		cout << "database::connect: " << (const char*)mysql_error(sqlSocket) << endl;
		throw exception();
	}
	if(mysql_real_connect(sqlSocket, host.c_str(), user.c_str(), pass.c_str(), name.c_str(), 0, 0, 0) == NULL) {
		cout << "database::connect: " << (const char*)mysql_error(sqlSocket) << endl;
		throw exception();
	}
	else connected = true;
}

vector<srObject> database::loadObjects(void) {
	vector<srObject> tmpObjects;
	if(connected) {
		MYSQL_ROW row;
		MYSQL_RES *res;
		string sqlCommand;
		srObject tmpObject;
		std::stringstream tmpKey;

		// load incremental or inicial if server just started
		sqlCommand = "SELECT id, character_id, type_id, status, x, y, name FROM objects ";
		if(mysql_query(sqlSocket, sqlCommand.c_str())==0) {
			res = mysql_store_result(sqlSocket);			// get result
			while((row = mysql_fetch_row(res)) != NULL) {	// grap row
				tmpObject.handle	= atoi(row[0]);		// unique from auto_id field
				tmpObject.owner		= atoi(row[1]);
				tmpObject.type		= atoi(row[2]);
				tmpObject.status	= atoi(row[3]);
				tmpObject.x			= atol(row[4]);
				tmpObject.y			= atol(row[5]);
				tmpObject.name 		= row[6];
				tmpObjects.push_back(tmpObject);
			}
			// important, otherwise SYNC error
			mysql_free_result(res);

		} else {
			cout << "database::loadObjects: " << (const char*)mysql_error(sqlSocket) << endl;
			throw exception();
		}
	}
	return tmpObjects;
}

/**
 * Takes a username and password and checks the dataset against the database, if valid it returns the object handle the character is set to
 *   @param User The username
 *   @param Pass The password
 */
long database::login(string user, string pass) {

    MYSQL_ROW row;
    MYSQL_RES *res;
    string sqlCommand;
    std::stringstream tmpCommand;
    string::size_type test;
    long retval=0;
    long resultCount=0;

    // TODO: i have no idea what i am doing here, let this be double checked

	test = user.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_");
	if(test != string::npos) {
		cout << "database::login: SQL injection detected" << endl; //TODO-3 kill client
	    return retval;
	}

	test = pass.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_");
	if(test != string::npos) {
		cout << "database::login: SQL injection detected" << endl; //TODO-3 kill client
	    return retval;
	}

    tmpCommand << "SELECT objects.id FROM objects, login, characters WHERE login.id = characters.login_id AND characters.id = objects.character_id AND username = '" << user << "' AND password = '" << pass << "'";
    sqlCommand = tmpCommand.str();


    if(mysql_query(sqlSocket, sqlCommand.c_str())==0) {

        res = mysql_store_result(sqlSocket);            // get result
        while((row = mysql_fetch_row(res)) != NULL) { // grap row
            retval = atoi(row[0]);
            resultCount++;
        }

        // important, otherwise SYNC error
        mysql_free_result(res);

        // more than one result must a NPC
        if(resultCount>1) retval = 0;

    }
    else {
        cout << "database::login: " << (const char*)mysql_error(sqlSocket) << endl;
    }
    return retval;
}

