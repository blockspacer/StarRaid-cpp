#include "client.hpp"


using namespace std;

int main(int argc, char *argv[]) {

    /// create the game engine
    client myClient;

    /// init the engine
    myClient.init("client.xml");


    int end=0;
    while(end==0)  {

        /// run the game
        end = myClient.run();   // reboot on 0 end on 1
    }

    /// init the engine
    myClient.stop();

    return 0;
}
