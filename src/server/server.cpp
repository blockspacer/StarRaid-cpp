#include "server.hpp"

using namespace std;

server::server(string pPath) {
	logfile.open("server.log", ios::out);
	logfile << "Init ... ";

	// load configs
	config.load(pPath);

	// load objects
	objLoad();

	// timers
	runtime=0;
	fps = 0;
	lps = 0;
	dps = 0;
	blps = 0;
	fpsSmooth = 0;
	lpsSmooth = 0;
	dpsSmooth = 0;
	blpsSmooth = 0;
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

		// caqche per second values
		fpsSmooth = fps;
		lpsSmooth = lps;
		dpsSmooth = rakTx+rakRx;
		blpsSmooth = blps;
		timers.tickSecond();
		runtime++;

		// some debugging
		logfile << "[Tick-" << runtime << "] ";
		logfile << "obj: " << objects.size() << ", ";
		logfile << "fps: " << fpsSmooth << ", ";
		logfile << "lps: " << lpsSmooth << ", ";
		logfile << "blps: " << blpsSmooth << ", ";
		logfile << "rx: " << rakRx << ", ";
		logfile << "tx: " << rakTx << endl;

		// reset counter
		fps = 0;
		lps = 0;
		dps = 0;
		blps = 0;
		rakTx = 0;
		rakRx = 0;

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

	objLoop();

	// sleep a micro second, about 10000 ticks/second
	usleep(1);
}

void server::objLoad(void) {
	db.connect("starraid","127.0.0.1","root","","3306");
	vector<srObject> tmp;
	vector<srObject>::iterator i;
	tmp = db.loadObjects();
	for(i = tmp.begin(); i != tmp.end(); ++i) {
		objects[(*i).handle] = (*i);
	}
}
/*
 * Updates relevant (near) objects list and processes
 * for each time one's object relation to each one near
 */
void server::objLoop(void) {
	long maxQue=0;
	long handle=0;

	// select next item in que by wait counter
	map<long, srObject>::iterator i;
	for(i=objects.begin(); i!=objects.end(); ++i) {
		(*i).second.queCnt++;
		if(maxQue<(*i).second.queCnt) {
			maxQue = (*i).second.queCnt;
			handle = (*i).second.handle;
		}
		lps++;
	}
	
	// update neigbours every (0.5->2sec) or at network events 
	int xd,yd;
	float dist;
	if(objects[handle].queCntBig>=objects[handle].queCntBigMax) {
		blps++;
		objects[handle].queCntBig = 0;
		objects[handle].neighbours.clear();
		map<long, srObject>::iterator f;
		for(f=objects.begin(); f!=objects.end(); ++f) {
			xd = objects[handle].x - (*f).second.x;
			yd = objects[handle].y - (*f).second.y;
			dist = sqrt(xd*xd + yd*yd);
			if(dist<=50000) { ///TODO: adjust by config and dynamic calc prognosis
				objects[handle].neighbours.push_back((*f).second.handle);
			}
		}
	} else {
		objects[handle].queCntBig++;
	}

	// logic for all the neigbours
	vector<long> tmp = objects[handle].neighbours;
	vector<long>::iterator e;
	for(e=tmp.begin(); e!=tmp.end(); ++e) {
		//
	}

}


