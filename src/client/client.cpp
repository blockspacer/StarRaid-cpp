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

    render=1;

    screen = NULL;
    screenResX = 800;
    screenResY = 600;
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
    rakPeer->Connect("127.0.0.1", 60000, 0,0);
}

void client::initGFX(void) {

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
                        screenResX=1440;
                        screenResY=900;
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
    } else {
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


    // render when onmouseover effects AND if the mouse was just moved out (remove the last active tab)
    if(mouseX < 35 && mouseY > 128 && mouseY < 368) render=1;
    else if(memMouseX < 35) render=1;


    // remember the last mouse position
    memMouseX = mouseX;
    memMouseY = mouseY;

    // poll the network buffer
    netTick();

    // only draw when chnaged flag is set, otherwise just once a sec
    if(render) draw();

    // Does runtime calculations and adjustments
    tickRuntime();
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

    gfxSystem::instance()->fontArial12->drawtextPair(screen ,50, 40, "FPS: ", runtimeFpsSmooth);
    gfxSystem::instance()->fontArial12->drawtextPair(screen ,50, 55, "Renders: ", runtimeRenderSmooth);
    gfxSystem::instance()->fontArial12->drawtextPair(screen ,50, 70, "Runtime: ", runtimeCount);
    gfxSystem::instance()->fontArial12->drawtextPair(screen ,50, 85, "Delta: ", runtimeDelta);
    gfxSystem::instance()->fontArial12->drawtextPair(screen ,50, 100, "Sleep: ", runtimeSleep);

    // show new render
    SDL_Flip(screen);

    render=0;
    runtimeRenders++;
}

void client::netTick(void) {
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
            break;

            case ID_NEW_INCOMING_CONNECTION:
                cout << "netTick: A connection is incoming." << endl;
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

void client::netRead(RakNet::Packet *packet) {
    int messageType=0;
    unsigned char systemType=0;
    RakNet::RakString tmpStr;

    // start reading
    RakNet::BitStream myBitStream(packet->data, packet->length, false);
    myBitStream.Read(systemType); // always 77=ID_USER_PACKET_ENUM
    myBitStream.Read(messageType);

    switch(messageType) {

        case PONG:
            cout << "netRead: got PONG." << endl;
        break;

        case VERSION_ASK:
            //netSend(VERSION_ASK, rakPacket->systemAddress);
        break;

        default:
            cout << "netRead: unknown '" << (int)messageType << endl;
        break;
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

        case PING:
            Priority=HIGH_PRIORITY;
        break;

        case PONG:
            Priority=HIGH_PRIORITY;
        break;

        case VERSION_ASK:
            //
        break;

        default:
            cout << "send: unknown: '" << (int)messageType << endl;
        break;
    }

    // Senden
    rakPeer->Send(myBitStream, Priority, RELIABLE, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}
