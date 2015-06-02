#include "srConfig.hpp"

/**
 * Set all variables to zero
 */
srConfig::srConfig() {
	loaded=false;
}

srConfig::~srConfig() {
	//dtor
}

/**
 * The config system load methos is quite important, first it loads all
 * the key/value based config strings out of the config file and stores
 * them in a key/value based datapool.
 * In the second part ge graps the config strings that he needs and copy
 * them over into the variables, if no key is found it uses the default
 * setting.
 */
void srConfig::load(string pPath) {

	ifstream ifs( pPath.c_str() );
	string temp;
	unsigned int i;
	bool flagReadValue;
	string key, value;


	while( getline( ifs, temp ) ) {

		key="";
		value="";
		flagReadValue=0;

		// check if not empty
		if(temp.length()>0) {

			// check if not a comment
			if(temp[0] != '#') {

				// loop for all characters
				for(i=0; i<temp.length(); i++) {
					if(temp[i] == '=') flagReadValue=1;
					if(flagReadValue==0) key += temp[i];
					else if(temp[i] != '=') value += temp[i];
				}

				// add Key/Value pair to the cache
				cache.insert(pair<string, string>(key, value));
			}
		}
	}

	loaded=true;
}


/**
 * Sets the debugging flag
 */
void srConfig::init(bool flagDebuging) {
	//
}


/**
 * This method is grapping an integer value out of the datapool, it does a
 * atoi cast.
 *   @warning the atoi cast is not at all caught by an exeption
 *   @param Key The key-string
 *   @param defaultValue if the key is not found, it will return the default value
 */
int srConfig::getValue(string Key, int defaultValue) {
	isLoaded();
	map<string, string>::iterator i = cache.find(Key);
	if(i == cache.end()) {
		return defaultValue;
	}
	return atoi(i->second.c_str());
}

/**
 * Simmilar to getValueInt() iz just returns a string value
 *   @param Key The key-string
 *   @param defaultValue if the key is not found, it will return the default value
 */
string srConfig::getValue(string Key, string defaultValue) {
	isLoaded();
	map<string, string>::iterator i = cache.find(Key);
	if(i == cache.end()) {
		return defaultValue;
	}
	return i->second;
}

/**
 * Check if the congif is already loaded
 */
void srConfig::isLoaded(void) {
	if(loaded==false) {
		cout << "srConfig::isLoaded: The config was not loaded " << endl;
		throw exception();
	}
}



