#include "Arduino.h"
#include <ESP8266WiFi.h>
#include "yahooWeather.h"

yahooWeather::yahooWeather(){		
	_old = millis();
}

void yahooWeather::Start(WiFiClient client,int refreshRate){
	_client = client;
	_refreshRate = refreshRate;			
	delay(5);
	if(millis()>_old){
		_old = millis()+_refreshRate;
			_json = "";
		    _error = "";	
		    const char* host = "query.yahooapis.com";		    			   			    
		    const int httpPort = 80;
		    if (!_client.connect(host, httpPort)) {
		      _error = "connection failed";
		      return;
		    }
		    String url = "/v1/public/yql?q=select%20*%20from%20weather.forecast%20where%20woeid%20in%20(select%20woeid%20from%20geo.places(1)%20where%20text%3D%22london%22)&format=json&env=store%3A%2F%2Fdatatables.org%2Falltableswithkeys";
		    _client.print(String("GET ") + url + " HTTP/1.1\r\n" +
		                 "Host: " + host + "\r\n" +
		                 "Connection: close\r\n\r\n");
		    unsigned long timeout = millis();
		    while (_client.available() == 0) {
		    	delay(100);
		      if (millis() - timeout > 5000) {
		        _error = "_client timeout";
		        _client.stop();
		        return;
		      }
		    }

		    while (_client.available()) {
		      String payload = _client.readString();
		      String header = payload.substring(0, payload.indexOf("\r\n\r\n")), body = payload.substring(payload.indexOf("\r\n\r\n") + 9, payload.length() - 6);
		      _json = body;      
		    }
	}		
}

String yahooWeather::getData(){
	if(_error != "") return _error;
	else return _json;
}