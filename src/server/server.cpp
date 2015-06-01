#include "server.hpp"

using namespace std;

server::server(string pPath) {
	logfile.open("server.log", ios::out);
	logfile << "Init ... ";

	// load configs
	config.load(pPath);

	// load objects
	loadObjects();

	// timers
	runtime=0;
	fps = 0;
	lps = 0;
	dps = 0;
	fpsSmooth = 0;
	lpsSmooth = 0;
	dpsSmooth = 0;
	timers.addTimer("second", 1000); //   1 times per second
	timers.addTimer("tenth",   100); //  10 times per second
	timers.addTimer("100th",    10); // 100 times per second

	// start network
	rakInit(true);

	logfile << "OK" << endl;
}

server::~server() {
	// disconnect and end gracefully
}

void server::tick(void) {
	fps++;

	// second
	if( timers.checkTimer("second") ) {
		fpsSmooth = fps; // ticks per second
		lpsSmooth = lps; // loops per second
		dpsSmooth = dps; // network packages per second
		timers.tickSecond();
		runtime++;
		fps = 0;
		lps = 0;
		dps = 0;
		logfile << "[Tick-" << runtime << "] fps: " << fpsSmooth << ", lps: " << lpsSmooth << ", dps: " << dpsSmooth << endl;

		// check on network connections
		rakNetCheckClients();
	}

	// 10x second
	if( timers.checkTimer("tenth") ) {
		//
	}

	// 100x second
	if( timers.checkTimer("100th") ) {
		//
	}

	// poll for network events
	rakTick();

	// sleep a micro second, about 10000 ticks/second
	usleep(1);
}

void server::loadObjects(void) {
	db.connect("starraid","127.0.0.1","root","","3306");
	vector<srObject> tmp;
	vector<srObject>::iterator i;
	tmp = db.loadObjects();
	for(i = tmp.begin(); i != tmp.end(); ++i) {
		objects[(*i).handle] = (*i);
	}
}



