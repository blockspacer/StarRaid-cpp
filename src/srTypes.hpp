enum netMessageType {
	PING,                 //!< will request a PONG response
	PONG,                 //!< answer to the PING request
	VERSION_ASK,          //!< ask for the version
	VERSION_ANSWER,       //!< answer the version number
	LOGIN_ASK,            //!< ask for login
	LOGIN_ANSWER,         //!< answer with login details (name, pass)
	OBJ_BEACON,           //!< has basic object infos, like pos, type, name, id
	OBJ_SELF,             //!< same as beacon but for the object to itself
	CMD_MOVE,             //!< tells where a move_to click was pointed to
	DETAILS_ASK,          //!< ask for more details about an object
	DETAILS_ANSWER,       //!< gives more details about an object depending how much the requester is allowed to see
	TERMINATE,            //!< ask the client to terminate itself
	CHAT,                 //!< a chat message send my client, server adds the sender info and send to others back
	OBJ_TYPE,             //!< send a objectType tp the client
	ACTION                //!< ACTION: a basic shot (server will answer with details of involved objects)
};