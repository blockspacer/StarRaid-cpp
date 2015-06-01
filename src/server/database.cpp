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
	if(mysql_real_connect(sqlSocket, "127.0.0.1", "root", "member", "starraid", 0, 0, 0) == NULL) {
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
				tmpObject.handle       = atoi(row[0]);		// unique from auto_id field
				tmpObject.character_id = atoi(row[1]);
				tmpObject.type         = atoi(row[2]);
				tmpObject.status       = atoi(row[3]);
				tmpObject.x            = atol(row[4]);
				tmpObject.y            = atol(row[5]);
				tmpObject.name         = row[6];
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

