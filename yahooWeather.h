#ifndef yahooWeather_h
#define yahooWeather_h

#include "Arduino.h"

class yahooWeather{	
	public:
		yahooWeather();		
		void Start(WiFiClient client,int refreshRate);
		String getData();
	private:
		WiFiClient _client;
		String  _json, _error;
		int _old, _refreshRate;
};

#endif