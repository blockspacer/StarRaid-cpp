#include "srNetworkClient.hpp"


/**
 * Set inicial values to NULL
 */
srNetworkClient::srNetworkClient() {
    cntLaag=0;
    cntFaildLogin=0;
    Status=0;
    inUse=0;
//    name[128];
    objektID=0;
}

/**
 * Long description
 */
srNetworkClient::~srNetworkClient() {
    //
}
