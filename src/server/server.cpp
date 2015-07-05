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

	logfile << "OK" << endl;
}

server::~server() {
	// terminate all clients
	for(map<RakNet::SystemAddress, networkClient>::iterator i=clients.begin(); i!=clients.end(); ++i) {
		netSend(TERMINATE,(*i).first);
	}

}

void server::tick(void) {
	fps++;

	// second
	if( timers.checkTimer("second") ) {

		// caqche per second values
		fpsSmooth = fps;
		lpsSmooth = lps;
		dpsSmooth = netTx + netRx;
		blpsSmooth = blps;
		timers.tickSecond();
		runtime++;

		// some debugging
		logfile << "[Tick-" << runtime << "] ";
		logfile << "obj: " << objects.size() << ", ";
		logfile << "fps: " << fpsSmooth << ", ";
		logfile << "lps: " << lpsSmooth << ", ";
		logfile << "blps: " << blpsSmooth << ", ";
		logfile << "rx: " << netRx << ", ";
		logfile << "tx: " << netTx << endl;

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
	string name = config.getValue("Name", "starraid");
	string user = config.getValue("User", "root");
	string pass = config.getValue("Pass", "");
	string host = config.getValue("Host", "127.0.0.1");
	string port = config.getValue("Port", "3306");
	db.connect(name,host,user,pass,port);
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
	RakNet::SocketDescriptor tmpSocketDescriptor;
	tmpSocketDescriptor.port = 60000;
	strcpy(tmpSocketDescriptor.hostAddress, "0");
	rakPeer->Startup(1024, &tmpSocketDescriptor, 1);
	rakPeer->SetMaximumIncomingConnections(1024);
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
	cout << "netClientTerminate: Client Added." << endl;
}

void server::netClientTerminate(RakNet::SystemAddress address) {
	netSend(TERMINATE,address);				// send terminate-notification to client
	clients.erase(address);					// remove client from pool
	rakPeer->CloseConnection(address,1);	// close peers conection
	cout << "netClientTerminate: Client Terminated." << endl;
}

void server::netTick(void) {
	rakPacket=rakPeer->Receive();
	int identifier=0;
	while(rakPacket) {
		identifier = (int)rakPacket->data[0];
		switch(identifier) {
			case ID_REMOTE_DISCONNECTION_NOTIFICATION:
				cout << "netTick: Another client has disconnected." << endl;
			break;

			case ID_REMOTE_CONNECTION_LOST:
				cout << "netTick: Another client has lost the connection." << endl;
			break;

			case ID_REMOTE_NEW_INCOMING_CONNECTION:
				cout << "netTick: Another client has connected." << endl;
			break;

			case ID_CONNECTION_REQUEST_ACCEPTED:
				cout << "netTick: A new connection." << endl;
				netSend(VERSION_ASK, rakPacket->systemAddress);
			break;

			case ID_NEW_INCOMING_CONNECTION:
				cout << "netTick: A connection is incoming." << endl;
				netClientAdd(rakPacket);
			break;

			case ID_NO_FREE_INCOMING_CONNECTIONS:
				cout << "netTick: The server is full." << endl;
			break;

			case ID_DISCONNECTION_NOTIFICATION:
				cout << "netTick: A client has disconnected." << endl;
			break;

			case ID_CONNECTION_LOST:
				cout << "netTick: A client lost the connection." << endl;
			break;


			//**** custom ****
			case ID_USER_PACKET_ENUM:
				netRead(rakPacket);
			break;

			default:
				cout << "netTick: unknown: " << identifier << endl;
			break;
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

		case PING:
			netSend(PONG, rakPacket->systemAddress);
			clients[packet->systemAddress].cntLaag = 0;
		break;

		default:
			cout << "netRead: unknown '" << messageType << endl;
		break;
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

		case PONG:
			Priority=HIGH_PRIORITY;
		break;

		case VERSION_ASK:
			//
		break;

		case TERMINATE:
			Priority=HIGH_PRIORITY;
		break;

		default:
			cout << "netSend: unknown: '" << messageType << endl;
		break;
	}

	// Senden
	rakPeer->Send(myBitStream, Priority, RELIABLE, 0, address, true);
}

