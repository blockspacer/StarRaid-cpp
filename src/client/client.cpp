#include "client.hpp"

client::client() {
	mouseX = 0;
	mouseY = 0;
	mouseZ = 0;
	memMouseX = 0;
	memMouseY = 0;
	mouseButton = 0;
	memMouseDownX = -1;
	memMouseDownY = -1;
	moveX = -1;
	moveY = -1;

	version = 101;
	menueHover = 0;
	render=1;

	selfHandle = 0;
	selfX = 0;
	selfY = 0;

	screen = NULL;
	screenResX = 1024;
	screenResY = 768;
	screenResHalfX = 0;
	screenResHalfY = 0;
	screenFull = 0;
	screenDepth = 32;

	runtimeSleep=1000;
	runtimeCount=0;
	runtimeFlops=0;
	runtimeRenders=0;
	runtimeFpsSmooth=0;
	runtimeRenderSmooth=0;
	runtimeDelta=0;
	runtimeTime=0;
	runtimeOptimumFPS=60;

	rakPacket = NULL;
	RakNet::SocketDescriptor tmpSocketDescriptor;
	rakPeer = RakNet::RakPeerInterface::GetInstance();
	rakPeer->Startup(1,&tmpSocketDescriptor, 1);

}

client::~client() {
	//
}

void client::init(string pPath) {
	config.load(pPath);
	screenResX = config.getValue("ResolutionX", 640);
	screenResY = config.getValue("ResolutionY", 480);
	screenFull = config.getValue("ResolutionFullscreen", 0);
	screenDepth = config.getValue("ResolutionDepth", 32);

	// start network
	rakPeer->Connect(config.getValue("Server","127.0.0.1").c_str(), 60000, 0,0);
}

void client::initGFX(void) {
	screenResHalfX = (int)screenResX / 2;
	screenResHalfY = (int)screenResY / 2;

	// set SDL exit routines
	atexit(TTF_Quit);
	atexit(SDL_Quit);

	// init the SDL library
	if(SDL_Init(SDL_INIT_VIDEO) < 0 ) {
		cout << "engine::initSDL: Couldn't initialize SDL: " << SDL_GetError() << endl;
		exit(1);
	}

	// init fonts
	if(TTF_Init() < 0 ) {
		cout << "engine::initSDL: Couldn't initialize TTF_Init: " << SDL_GetError() << endl;
		exit(1);
	}

	// set main screen
	if(screenFull) screen = SDL_SetVideoMode(screenResX, screenResY, screenDepth, SDL_DOUBLEBUF | SDL_HWSURFACE | SDL_FULLSCREEN);
	else screen = SDL_SetVideoMode(screenResX, screenResY, screenDepth, SDL_DOUBLEBUF | SDL_HWSURFACE);
	if (screen == NULL) {
		cout << "engine::initSDL: Couldn't set Resolution (" << screenResX << "x" << screenResY << "):" << SDL_GetError() << endl;
		exit(1);
	}

	// load images & precalculate menues aso.
	gfxSystem::instance()->init(screenResX, screenResY);

	// generate stars
	starsInit();
}

bool client::run(void) {

	// init SDL
	initGFX();

	// game loop
	while(1) {

		SDL_Event event;
		if(SDL_PollEvent(&event)) {

			switch( event.type ){
				case SDL_KEYDOWN:


				switch(event.key.keysym.sym) {

					case SDLK_ESCAPE:
						return true; // EXIT the game
					break;

					case SDLK_1:
						screenResX=800;
						screenResY=600;
						screenFull=0;
						render=1;
						return false;
					break;

					case SDLK_2:
						screenResX=1024;
						screenResY=768;
						screenFull=0;
						render=1;
						return false;
					break;

					case SDLK_3:
						screenResX=1920;
						screenResY=1080;
						screenFull=1;
						render=1;
						return false;
					break;

					// default just because of compiler warning
					default:
					break;

				}
				break;

			}
		}

		// game logic
		tick();
	}
}

void client::tick(void) {
	runtimeFlops++;

	// get latest mouse position (does not work over mouse motion, to slow, better no buffer)
	SDL_GetMouseState(&mouseX, &mouseY);

	// Mouse CLICK events
	if(SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(1)) {
		// Lock IN Mouse
		if(memMouseDownX==-1 && memMouseDownY==-1) {
			memMouseDownX = mouseX;
			memMouseDownY = mouseY;
		}
	} else { // no system mouse
		// Free Mouse And generate mouse up event
		if(memMouseDownX!=-1 || memMouseDownY!=-1) {
			moveX = memMouseDownX;
			moveY = memMouseDownY;

			// set back to default (LOCK)
			memMouseDownX = -1;
			memMouseDownY = -1;

			// render
			render=1;
		}
	}

	// remember the last mouse position
	memMouseX = mouseX;
	memMouseY = mouseY;

	// poll the network buffer
	netTick();

	// check on menue action
	tickMenue();

	// only draw when chnaged flag is set, otherwise just once a sec
	if(render) draw();

	// Does runtime calculations and adjustments
	tickRuntime();
}

void client::tickMenue(void) {
	int oldHover = menueHover;
	if(memMouseX < 35) {
		int i;
		for(i=0;i<=7;i++) {
			if( memMouseY > (i*30)+128 && memMouseY < (i*30)+128+30 ) {
				menueHover = i+1;
			}
		}
	} else {
		menueHover = 0;
	}
	if(oldHover!=menueHover) {
		gfxSystem::instance()->createMenue(screenResX, screenResY, menueHover);
		render=1;
	}
}

void client::tickRuntime(void) {
	int currentTime = (uintmax_t)time(NULL);
	if(runtimeTime != currentTime) { // every second
		runtimeTime = currentTime;
		runtimeFpsSmooth = runtimeFlops;
		runtimeRenderSmooth = runtimeRenders;
		runtimeFlops = 0;
		runtimeRenders = 0;
		runtimeCount++;

		// network keep alive ping
		netSend(PING);

		// adjust the runtimeSleep time and free CPU (TODO-1: make in formula like x^2*5)
		int temp=runtimeOptimumFPS-runtimeFpsSmooth;
		if(temp<-30) runtimeSleep += 4000;
		else if(temp<-20) runtimeSleep += 2000;
		else if(temp<-10) runtimeSleep += 1000;
		else if(temp<-5) runtimeSleep += 500;
		else if(temp<-3) runtimeSleep += 300;
		else if(temp<-2) runtimeSleep += 200;
		else if(temp<-0) runtimeSleep += 100;
		else if(temp>30) runtimeSleep -= 4000;
		else if(temp>20) runtimeSleep -= 2000;
		else if(temp>10) runtimeSleep -= 1000;
		else if(temp>5) runtimeSleep -= 500;
		else if(temp>3) runtimeSleep -= 300;
		else if(temp>2) runtimeSleep -= 200;
		else if(temp>0) runtimeSleep -= 100;

		// check for max break to keep system 'nice'
		if(runtimeSleep<=0) runtimeSleep = 10;
		if(runtimeSleep>=30000) runtimeSleep = 30000;

		render=1;
	}
	usleep(runtimeSleep);
}

void client::draw(void) {

	// empty
	SDL_FillRect(screen, NULL, 0x000000);

	gfxSystem::instance()->fontArial12->drawtextPair(screen ,50, 20, "FPS: ", runtimeFpsSmooth);
	gfxSystem::instance()->fontArial12->drawtextPair(screen ,50, 35, "Renders: ", runtimeRenderSmooth);
	gfxSystem::instance()->fontArial12->drawtextPair(screen ,50, 50, "Runtime: ", runtimeCount);
	gfxSystem::instance()->fontArial12->drawtextPair(screen ,50, 65, "Delta: ", runtimeDelta);
	gfxSystem::instance()->fontArial12->drawtextPair(screen ,50, 80, "Sleep: ", runtimeSleep);
	gfxSystem::instance()->fontArial12->drawtextPair(screen ,50, 95, "Hover: ", menueHover);


	// draw other objects
	drawGfx();

	// show new render
	SDL_Flip(screen);

	render=0;
	runtimeRenders++;
}

void client::drawGfx(void) {

	// draw stars first
	starsDraw();

	// draw stars first
	objectsDraw();

	// fixed elements
	gfxSystem::instance()->poolGet("radar")->draw(screen);

	// menu
	gfxSystem::instance()->sfMenue->draw(screen);

	// messageBox
	gfxSystem::instance()->sfMsg->draw(screen);
}

//TODO: move resulution projection into the netReadBacon
void client::objectsDraw(void) {
	std::stringstream tmpMessage;
	int x = 0;
	int y = 0;
    int cnt=205;
    tmpMessage.str("");
    map<long, srObject>::iterator i;
    for( i=objects.begin(); i!=objects.end(); ++i) {

    	// infos
    	tmpMessage.str("");
        tmpMessage << "obj["<<(*i).second.handle<<"]: t="<<(*i).second.type<<" x="<<(*i).second.x<<" y="<<(*i).second.y;
        gfxSystem::instance()->fontArial12->drawtextPair(screen ,50, cnt, "  - ", tmpMessage.str());
        cnt += 15;

        // objects
        if((*i).first != selfHandle) {
			int x = screenResHalfX + selfX - (*i).second.x;
			int y = screenResHalfY + selfY - (*i).second.y;
        	gfxSystem::instance()->poolGet("SCH1")->drawCentered(screen, x, y, 0);
        }
    }
}

//void client::objectsDraw(void) {
	//
//}

void client::netTick(void) {
	rakPacket=rakPeer->Receive();
	int identifier=0;
	while(rakPacket) {
		identifier = (int)rakPacket->data[0];

		switch(identifier) {

			case ID_CONNECTION_REQUEST_ACCEPTED: {
				netSend(CLIENT_CONNECTED);
				break; 
			}

			case ID_NO_FREE_INCOMING_CONNECTIONS: {
				cout << "netTick: The server is full." << endl;
				break; 
			}

			case ID_CONNECTION_LOST: {
				cout << "netTick: A client lost the connection." << endl;
				break; 
			}

			case ID_USER_PACKET_ENUM: {
				netRead(rakPacket);
				break;
			}

			default: {
				cout << "netTick: Not Handled: " << rakNetMessageNames[identifier] << endl;
				break;
			}
		}

		// pop the package out of the buffer
		rakPeer->DeallocatePacket(rakPacket);
		rakPacket=rakPeer->Receive();   // fetch anotherone
	}
}

void client::netRead(RakNet::Packet *packet) {
	int messageType=0;
	unsigned char systemType=0;
	RakNet::RakString tmpStr;

	// start reading
	RakNet::BitStream myBitStream(packet->data, packet->length, false);
	myBitStream.Read(systemType); // always 77=ID_USER_PACKET_ENUM
	myBitStream.Read(messageType);

	switch(messageType) {

		case PING: {
			netSend(PONG);
			break;
		}

		case PONG: {
			break;
		}

		case VERSION_ASK: {
			netSend(VERSION_ANSWER);
			break;
		}

		case LOGIN_ASK: {
			netSend(LOGIN_ANSWER);
			break;
		}

		case OBJ_BEACON: {
			netReadBeacon(packet, 0);
			break;
		}

		case OBJ_SELF: {
			netReadBeacon(packet, 1);
			break;
		}

		default: {
			cout << "netRead: Not Handled: " << netMessageTypenames[messageType] << endl;
			break;
		}
	}
}

void client::netSend(int messageType) {
	std::stringstream tmpMessage;
	unsigned char systemType=ID_USER_PACKET_ENUM;
	PacketPriority Priority=LOW_PRIORITY;

	RakNet::BitStream *myBitStream = new RakNet::BitStream;
	myBitStream->Write(systemType);
	myBitStream->Write(messageType);

	switch(messageType) {

		case CLIENT_CONNECTED: {
			break;
		}

		case PING: {
			Priority=HIGH_PRIORITY;
			break;
		}

		case PONG: {
			Priority=HIGH_PRIORITY;
			break;
		}

		case VERSION_ANSWER: {
			myBitStream->Write(version);
			break;
		}

		case LOGIN_ANSWER: {
			RakNet::RakString user = config.getValue("User","none").c_str();
			RakNet::RakString pass = config.getValue("Pass","none").c_str();
			myBitStream->Write(user);
			myBitStream->Write(pass);
			break;
		}

		default: {
			cout << "netSend: Not Handled: " << netMessageTypenames[messageType] << endl;
			break;
		}
	}

	// Senden
	rakPeer->Send(myBitStream, Priority, RELIABLE, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}

void client::netReadBeacon(RakNet::Packet *packet, bool flagSelf) {

	// stuff need to unpack the bitstream
	int customType=0;
	unsigned char systemType=0;


	// start reading
	RakNet::BitStream myBitStream(packet->data, packet->length, false);
	myBitStream.Read(systemType); // allways 77=ID_USER_PACKET_ENUM
	myBitStream.Read(customType);

	// create temprary object
	srObject tmpObject;

	// Beacon data
	myBitStream.Read(tmpObject.handle);
	myBitStream.Read(tmpObject.x);
	myBitStream.Read(tmpObject.y);
	myBitStream.Read(tmpObject.type);
	myBitStream.Read(tmpObject.status);

	// tell the object system witch object is the players
	if(flagSelf) {
		selfHandle = tmpObject.handle;
		selfX = tmpObject.x;
		selfY = tmpObject.y;
		render=1;
	}
		
	objects[tmpObject.handle] = tmpObject;

	// rerender on incoming package
	//TODO: render non self when in screen
}

void client::starsInit(void) {

	// delte all stars because of resolution change
	stars.clear();

	structStar temp;
	int i;
	float pIntensity=screenResX*screenResY/150000;

	// small
	for(i=0;i<(int)(pIntensity*25);i++) {
		temp.typ = 3;
		temp.x = rand()%screenResX;
		temp.y = rand()%screenResY;
		stars.push_back(temp);
		starsCount++;
	}

	// middle
	for(i=0;i<(int)(pIntensity*20);i++) {
		temp.typ = 2;
		temp.x = rand()%screenResX;
		temp.y = rand()%screenResY;
		stars.push_back(temp);
		starsCount++;
	}

	// big
	for(i=0;i<(int)(pIntensity*15);i++) {
		temp.typ = 1;
		temp.x = rand()%screenResX;
		temp.y = rand()%screenResY;
		stars.push_back(temp);
		starsCount++;
	}
}

/**
 * Will calculate the movement of the stars
 *   @param moveX The Movement rate of the selfObject from last calculation in X
 *   @param moveY The Movement rate of the selfObject from last calculation in Y
 */
void client::starsMove(int moveX, int moveY) {

	if(moveX != 0 || moveY != 0) {

		vector<structStar>::iterator i;
		for( i=stars.begin(); i!=stars.end(); ++i) {

			int mod = 0;
			int pFactor=1;

			// flip edge
			if((*i).y < 0) (*i).y = screenResY;
			if((*i).x < 0) (*i).x = screenResX;
			if((*i).y > screenResY) (*i).y = 0;
			if((*i).x > screenResX) (*i).x = 0;

			// Ebenen modifikator
			if((*i).typ==3) mod = 15;
			if((*i).typ==2) mod = 6;
			if((*i).typ==1) mod = 2;

			(*i).x += ((float)moveX/mod)*pFactor;
			(*i).y += ((float)moveY/mod)*pFactor;
		}
	}
}

/**
 * Is drawing the stars ito the screen pinter
 */
void client::starsDraw(void) {

	// star background
	vector<structStar>::iterator i;
	for( i=stars.begin(); i!=stars.end(); ++i) {
		if((*i).typ==3) gfxSystem::instance()->poolGet("star_big")->drawCentered(screen, (*i).x, (*i).y, 0);
		if((*i).typ==2) gfxSystem::instance()->poolGet("star_middle")->drawCentered(screen, (*i).x, (*i).y, 0);
		if((*i).typ==1) gfxSystem::instance()->poolGet("star_small")->drawCentered(screen, (*i).x, (*i).y, 0);
	}

}

