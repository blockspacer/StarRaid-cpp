#include "server.hpp"

using namespace std;

server::server(string pPath) {
	logfile.open("server.log", ios::out);

	// load configs
	config.load(pPath);
	debugLevel=config.getValue("debugLevel", 1); // 1: basic, 2: errors, 3: infos, 4: performance
	if(debugLevel>=1) logfile << "server::server ... START" << endl;

	// connect to DB
	string name = config.getValue("Name", "starraid");
	string user = config.getValue("User", "root");
	string pass = config.getValue("Pass", "");
	string host = config.getValue("Host", "127.0.0.1");
	string port = config.getValue("Port", "3306");
	db.connect(name,host,user,pass,port);

	// load objects
	objLoad();

	// version
	version=101;

	// timers
	runtime=0;
	calcDist = config.getValue("calcDist", 50000);
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
	rakPacket = NULL;
	rakPeer = RakNet::RakPeerInterface::GetInstance();
	netTx = 0;
	netRx = 0;
	netInit();

	if(debugLevel>=1) logfile << "server::server ... DONE" << endl;
}

server::~server() {
	// terminate all clients
	for(map<RakNet::SystemAddress, networkClient>::iterator i=clients.begin(); i!=clients.end(); ++i) {
		netSend(TERMINATE,(*i).first);
	}
	cout << endl; // fix prompt ;-)
}

void server::tick(void) {
	fps++;

	// second
	if( timers.checkTimer("second") ) {

		// cache per second values
		fpsSmooth = fps;
		lpsSmooth = lps;
		dpsSmooth = netTx + netRx;
		blpsSmooth = blps;
		timers.tickSecond();
		runtime++;

		// some debugging
		if(debugLevel>=4) {
			logfile << "[Tick-" << runtime << "] ";
			logfile << "obj: " << objects.size() << ", ";
			logfile << "fps: " << fpsSmooth << ", ";
			logfile << "lps: " << lpsSmooth << ", ";
			logfile << "blps: " << blpsSmooth << ", ";
			logfile << "rx: " << netRx << ", ";
			logfile << "tx: " << netTx << endl;
		}

		// reset counter
		fps = 0;
		lps = 0;
		dps = 0;
		blps = 0;

		// check on network connections
		netClientCheck();
	}

	// 10x second
	if( timers.checkTimer("tenth") ) {
		//
	}

	// 100x second
	if( timers.checkTimer("100th") ) {
		//
	}

	// check network
	netTick();


	// main calculation (ext to network distribution)
	objLoop();

	// sleep a micro second, about 10000 ticks/second
	usleep(1);
}

void server::objLoad(void) {
	if(debugLevel>=1) logfile << "server::objLoad ... ";
	vector<srObject> tmp;
	vector<srObject>::iterator i;
	tmp = db.loadObjects();
	for(i = tmp.begin(); i != tmp.end(); ++i) {
		objects[(*i).handle] = (*i);
	}
	if(debugLevel>=1) logfile << "OK" << endl;
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
			if(dist<=calcDist) { ///TODO: adjust by config and dynamic calc prognosis
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

void server::netInit(void) {
	if(debugLevel>=1) logfile << "server::netInit ... ";
	RakNet::SocketDescriptor tmpSocketDescriptor;
	tmpSocketDescriptor.port = 60000;
	strcpy(tmpSocketDescriptor.hostAddress, "0");
	rakPeer->Startup(1024, &tmpSocketDescriptor, 1);
	rakPeer->SetMaximumIncomingConnections(1024);
	if(debugLevel>=1) logfile << "OK" << endl;
}

void server::netClientCheck(void) {
	RakNet::SystemAddress tmpSystemAddress;
	bool flagDelete=0;
	for( map<RakNet::SystemAddress, networkClient>::iterator i=clients.begin(); i!=clients.end(); ++i) {
		(*i).second.cntLaag++;
		if((*i).second.cntLaag >= 15) {
			tmpSystemAddress = (*i).first;
			flagDelete=1;
		}
	}
	// delete one per secon is good enough
	if(flagDelete) netClientTerminate(tmpSystemAddress);
}

void server::netClientAdd(RakNet::Packet *packet) {
	networkClient tmpClient;
	clients.insert(pair<RakNet::SystemAddress, networkClient>(packet->systemAddress, tmpClient));
	if(debugLevel>=3) logfile << "server::netClientAdd> " << packet->systemAddress.ToString() << endl;
}

void server::netClientTerminate(RakNet::SystemAddress address) {
	netSend(TERMINATE,address);				// send terminate-notification to client
	clients.erase(address);					// remove client from pool
	rakPeer->CloseConnection(address,1);	// close peers conection
	if(debugLevel>=3) logfile << "server::netClientTerminate> " << address.ToString() << endl;
}

void server::netTick(void) {
	rakPacket=rakPeer->Receive();
	int identifier=0;
	while(rakPacket) {
		identifier = (int)rakPacket->data[0];
		switch(identifier) {

			case ID_NEW_INCOMING_CONNECTION: {
				netClientAdd(rakPacket);
				break;
			}

			case ID_CONNECTION_LOST: { // also timeout
				netClientTerminate(rakPacket->systemAddress);
				break;
			}

			case ID_USER_PACKET_ENUM: {
				netRead(rakPacket);
				break;
			}

			default: {
				if(debugLevel>=2) logfile << "server::netTick> Not Handled: " << rakNetMessageNames[identifier] << endl;
				break;
			}
		}

		// pop the package out of the buffer
		rakPeer->DeallocatePacket(rakPacket);
		rakPacket=rakPeer->Receive();   // fetch anotherone
	}
}

void server::netRead(RakNet::Packet *packet) {
	netRx++;
	std::stringstream tmpMessage;
	int messageType=0;
	unsigned char systemType=0;
	RakNet::RakString tmpStr;

	// start reading
	RakNet::BitStream myBitStream(packet->data, packet->length, false);
	myBitStream.Read(systemType); // always 77=ID_USER_PACKET_ENUM
	myBitStream.Read(messageType);

	switch(messageType) {

		case CLIENT_CONNECTED: {
			netSend(VERSION_ASK, rakPacket->systemAddress);
			break;
		}

		case PING: {
			//netSend(PONG, rakPacket->systemAddress); // no need to actully answer
			clients[packet->systemAddress].cntLaag = 0;
			break;
		}

		case PONG: {
			break;
		}

		case VERSION_ANSWER: {
			int tmpVersion=0;
			myBitStream.Read(tmpVersion);
			if(debugLevel>=3) logfile << "server::netRead> VERSION_ANSWER("<<tmpVersion<<"): ";
			if(tmpVersion==version) {
				netSend(LOGIN_ASK, rakPacket->systemAddress);
				if(debugLevel>=3) logfile << "OK" << endl;
			}
			else {
				netSend(VERSION_WRONG, rakPacket->systemAddress);
				if(debugLevel>=3) logfile << "FAILED" << endl;
			}
			break;
		}

		case LOGIN_ANSWER: {
			RakNet::RakString user, pass;
			myBitStream.Read(user);
			myBitStream.Read(pass);
			if(debugLevel>=3) logfile << "server::netRead> LOGIN_ANSWER("<<user<<"/"<<pass<<"): ";
			long id = db.login(user.C_String(), pass.C_String());
			if(id!=0) {
				clients[packet->systemAddress].name = user;
				clients[packet->systemAddress].objektID = id;
				objects[id].pilotAddress = packet->systemAddress;
				netAfterLogin(rakPacket->systemAddress);
				if(debugLevel>=3) logfile << "OK" << endl;
			} else {
				netSend(LOGIN_WRONG, rakPacket->systemAddress); //TODO-2: remember and bann at X times 
				if(debugLevel>=3) logfile << "FAILED" << endl;
			}
			break;
		}

		default: {
			if(debugLevel>=2) logfile << "server::netRead: Not Handled: " << netMessageTypenames[messageType] << endl;
			break;
		}
	}
}

void server::netSend(int messageType, RakNet::SystemAddress address) {
	netTx++;
	std::stringstream tmpMessage;
	unsigned char systemType=ID_USER_PACKET_ENUM;
	PacketPriority Priority=LOW_PRIORITY;

	RakNet::BitStream *myBitStream = new RakNet::BitStream;
	myBitStream->Write(systemType);
	myBitStream->Write(messageType);

	switch(messageType) {

		case PONG: {
			Priority=HIGH_PRIORITY;
			break;
		}

		case VERSION_ASK: {
			break;
		}

		case VERSION_WRONG: {
			break;
		}

		case LOGIN_ASK: {
			break;
		}

		case TERMINATE: {
			Priority=HIGH_PRIORITY;
			break;
		}

		default: {
			if(debugLevel>=2) logfile << "server::netSend: Not Handled: " << netMessageTypenames[messageType] << endl;
			break;
		}
	}

	// Senden
	rakPeer->Send(myBitStream, Priority, RELIABLE, 0, address, false);
}

void server::netAfterLogin(RakNet::SystemAddress address) {
	long id = clients[address].objektID;
	if(id!=0) {
		netSendBeacon(id,id,2); // self in full detail
		vector<long> tmp = objects[id].neighbours;
		vector<long>::iterator e;
		for(e=tmp.begin(); e!=tmp.end(); ++e) {
			netSendBeacon(id,(*e),1); // neighbour beacon only
		}
	}
}

void server::netSendBeacon(long deliverTo, long deliverObject, int level) {
	netTx++;

    // set whenever send rebular beacon or itself
    int customType;
    if(level==1) customType = OBJ_BEACON;
    if(level==2) customType = OBJ_SELF;

    // raknet system enum
    unsigned char systemType=ID_USER_PACKET_ENUM;

    // create new bitstream and set basic headers
    RakNet::BitStream *myBitStream = new RakNet::BitStream;
    myBitStream->Write(systemType);
    myBitStream->Write(customType);

    // Beacon data
    myBitStream->Write(objects[deliverObject].handle);
    myBitStream->Write(objects[deliverObject].x);
    myBitStream->Write(objects[deliverObject].y);
    myBitStream->Write(objects[deliverObject].type);
    myBitStream->Write(objects[deliverObject].status);

    // send bitstream out
    rakPeer->Send(myBitStream, HIGH_PRIORITY, RELIABLE, 0, objects[deliverTo].pilotAddress, false);
}
