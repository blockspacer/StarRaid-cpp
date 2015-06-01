#include "srNetwork.hpp"


srNetwork::srNetwork() {
	rakPacket = NULL;
	rakPeer = RakNet::RakPeerInterface::GetInstance();
	rakInitiated = false;
}

srNetwork::~srNetwork() {
	//
}

void srNetwork::rakInit(bool isServer) {
    RakNet::SocketDescriptor tmpSocketDescriptor;
	if(isServer) {
	    tmpSocketDescriptor.port = 60000;
	    strcpy(tmpSocketDescriptor.hostAddress, "0");
	    rakPeer->Startup(1024, &tmpSocketDescriptor, 1);
	    rakPeer->SetMaximumIncomingConnections(1024);
    } else {
		rakPeer->Startup(1,&tmpSocketDescriptor, 1);
    }
	rakInitiated=true;
}
