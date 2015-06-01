#include "srNetwork.hpp"


srNetwork::srNetwork() {
	rakPacket = NULL;
	rakPeer = RakNet::RakPeerInterface::GetInstance();
	rakIsServer = false;
	rakInitiated = false;
	rakConnected = false;
}

srNetwork::~srNetwork() {
	//
}

void srNetwork::rakInit(bool isServer) {
	RakNet::SocketDescriptor tmpSocketDescriptor;
	if(isServer) {
		rakIsServer = true;
		tmpSocketDescriptor.port = 60000;
		strcpy(tmpSocketDescriptor.hostAddress, "0");
		rakPeer->Startup(1024, &tmpSocketDescriptor, 1);
		rakPeer->SetMaximumIncomingConnections(1024);
	} else {
		rakPeer->Startup(1,&tmpSocketDescriptor, 1);
	}
	rakInitiated=true;
}

void srNetwork::rakConnect(string pHost) {
	if(rakInitiated) {
		rakPeer->Connect(pHost.c_str(), 60000, 0,0);
		rakConnected = true;
		//rakServerAddress
	}
}

void srNetwork::rakTick(void) {
	if(rakInitiated) {

		// recive packets
		rakPacket=rakPeer->Receive();
		std::stringstream tmpMessage;
		int identifier=0;


		// if something in the buffer fetch one and check it, only one a frame
		while(rakPacket) {

			identifier = (int)rakPacket->data[0];

			switch(identifier) {

				case ID_REMOTE_DISCONNECTION_NOTIFICATION:
					cout << "rakTick: Another client has disconnected." << endl;
				break;

				case ID_REMOTE_CONNECTION_LOST:
					cout << "rakTick: Another client has lost the connection." << endl;
				break;

				case ID_REMOTE_NEW_INCOMING_CONNECTION:
					cout << "rakTick: Another client has connected." << endl;
				break;

				case ID_CONNECTION_REQUEST_ACCEPTED:
					cout << "rakTick: A new connection." << endl;
//					rakNetSend(VERSION_ASK, rakPacket->systemAddress);
				break;

				case ID_NEW_INCOMING_CONNECTION:
					cout << "rakTick: A connection is incoming." << endl;
					rakNetClientAdd(rakPacket);
//					rakNetSend(VERSION_ASK, rakPacket->systemAddress);
				break;

				case ID_NO_FREE_INCOMING_CONNECTIONS:
					cout << "rakTick: The server is full." << endl;
				break;

				case ID_DISCONNECTION_NOTIFICATION:
					cout << "rakTick: A client has disconnected." << endl;
				break;

				case ID_CONNECTION_LOST:
					cout << "rakTick: A client lost the connection." << endl;
				break;


				//**** custom ****
				case ID_USER_PACKET_ENUM:
					rakNetRead(rakPacket);
				break;

				default:
					cout << "rakTick: unknown: " << (int)rakPacket->data[0] << endl;
				break;
			}

			// pop the package out of the buffer
			rakPeer->DeallocatePacket(rakPacket);
			rakPacket=rakPeer->Receive();   // fetch anotherone
		}
	}
}

void srNetwork::rakNetRead(RakNet::Packet *packet) {
	if(rakInitiated) {
		std::stringstream tmpMessage;
		int messageType=0;
		unsigned char systemType=0;
		RakNet::RakString tmpStr;

		// start reading
		RakNet::BitStream myBitStream(packet->data, packet->length, false);
		myBitStream.Read(systemType); // allways 77=ID_USER_PACKET_ENUM
		myBitStream.Read(messageType);

		switch(messageType) {

			case PING:
				rakNetSend(PONG, rakPacket->systemAddress);
				if(rakIsServer) clients[packet->systemAddress].cntLaag = 0;
			break;

			case PONG:
				//cout << "rakNetRead: got PONG." << endl;
			break;

			default:
				cout << "rakNetRead: unknown '" << (int)messageType << endl;
			break;
		}
	}
}

void srNetwork::rakNetSend(int messageType, RakNet::SystemAddress address) {
	if(rakInitiated) {
		std::stringstream tmpMessage;
		unsigned char systemType=ID_USER_PACKET_ENUM;
		PacketPriority Priority=LOW_PRIORITY;

		RakNet::BitStream *myBitStream = new RakNet::BitStream;
		myBitStream->Write(systemType);
		myBitStream->Write(messageType);

		switch(messageType) {

			case PING:
				Priority=HIGH_PRIORITY;
			break;

			case PONG:
				Priority=HIGH_PRIORITY;
			break;

			default:
				cout << "rakNetSend: unknown: '" << (int)messageType << endl;
			break;
		}

		// Senden
		rakPeer->Send(myBitStream, Priority, RELIABLE, 0, address, true);
	}
}

void srNetwork::rakNetCheckClients(void) {
	if(rakIsServer) {
		RakNet::SystemAddress tmpSystemAddress;
		bool flagDelete=0;
		for( map<RakNet::SystemAddress, srNetworkClient>::iterator i=clients.begin(); i!=clients.end(); ++i) {
			(*i).second.cntLaag++;
			if((*i).second.cntLaag >= 15) {
				tmpSystemAddress = (*i).first;
				flagDelete=1;
			}
		}
		// delete one per secon is good enough
		if(flagDelete) rakNetClientTerminate(tmpSystemAddress);
	}
}

void srNetwork::rakNetClientAdd(RakNet::Packet *packet) {
	if(rakIsServer) {
		srNetworkClient tmpClient;
		clients.insert(pair<RakNet::SystemAddress, srNetworkClient>(packet->systemAddress, tmpClient));
		cout << "rakNetClientTerminate: Client Added." << endl;
	}
}

void srNetwork::rakNetClientTerminate(RakNet::SystemAddress address) {
	if(rakIsServer) {
		rakNetSend(TERMINATE,address);			// send terminate-notification to client
		clients.erase(address);					// remove client from pool
		rakPeer->CloseConnection(address,1);	// close peers conection
		cout << "rakNetClientTerminate: Client Terminated." << endl;
	}
}

void srNetwork::rakNetAliasPing(void) {
	rakNetSend(PING, RakNet::UNASSIGNED_SYSTEM_ADDRESS);
}
