#include "srTypes.hpp"

srTypes::srTypes() {
    netMessageTypeString[0] = "PING";
    netMessageTypeString[1] = "PONG";
    netMessageTypeString[2] = "VERSION_ASK";
    netMessageTypeString[3] = "VERSION_ANSWER";
    netMessageTypeString[4] = "LOGIN_ASK";
    netMessageTypeString[5] = "LOGIN_ANSWER";
    netMessageTypeString[6] = "OBJ_BEACON";
    netMessageTypeString[7] = "OBJ_SELF";
    netMessageTypeString[8] = "CMD_MOVE";
    netMessageTypeString[9] = "DETAILS_ASK";
    netMessageTypeString[10] = "DETAILS_ANSWER";
    netMessageTypeString[11] = "TERMINATE";
    netMessageTypeString[12] = "CHAT";
    netMessageTypeString[13] = "OBJ_TYPE";
    netMessageTypeString[14] = "ACTION";
}

srTypes::~srTypes() {
    //dtor
}
