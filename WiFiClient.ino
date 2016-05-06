#include <ArduinoJson.h>
/*
    This sketch sends data via HTTP GET requests to data.sparkfun.com service.

    You need to get streamId and privateKey at data.sparkfun.com and paste them
    below. Or just customize this script to talk to other HTTP servers.

*/
#include <Adafruit_GFX.h>
#include <Adafruit_ssd1306syp.h>
#include <ESP8266WiFi.h>

#define SDA_PIN D1
#define SCL_PIN D2
Adafruit_ssd1306syp display(SDA_PIN, SCL_PIN);

const char* ssid     = "ITap Group";
const char* password = "gameshooting1";

const char* host = "query.yahooapis.com";

void setup() {
  Serial.begin(115200);
  delay(100);
  display.initialize();
  Serial.println("Connecting to " + String(ssid));
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Connected with IP address: ");
  Serial.println(WiFi.localIP());
  display.clear();
  display.println("test wifi client");
  display.update();
}

int value = 0;
String myJson, error;

int ht = millis(), ho = millis(), hdif = 5000;
void handleHttp() {
  ht = millis();
  if (ht > ho) {
    myJson = "";
    error = "";
    ho = millis() + hdif;
    ++value;

    WiFiClient client;
    const int httpPort = 80;
    if (!client.connect(host, httpPort)) {
      error = "connection failed";
      return;
    }
    String url = "/v1/public/yql?q=select%20*%20from%20weather.forecast%20where%20woeid%20in%20(select%20woeid%20from%20geo.places(1)%20where%20text%3D%22london%22)&format=json&env=store%3A%2F%2Fdatatables.org%2Falltableswithkeys";
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");
    unsigned long timeout = millis();
    while (client.available() == 0) {
      if (millis() - timeout > 5000) {
        error = "client timeout";
        client.stop();
        return;
      }
    }

    while (client.available()) {
      String payload = client.readString();
      String header = payload.substring(0, payload.indexOf("\r\n\r\n")), body = payload.substring(payload.indexOf("\r\n\r\n") + 9, payload.length() - 6);
      myJson = body;      
    }
  }
}



int current = millis(), old = millis(), old2 = millis();
void loop() {
  current = millis();
  handleHttp();

  if (current > old) {
    old = millis() + 5000;    
    DynamicJsonBuffer  jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(myJson);
    if (!root.success()) {
      Serial.println("parseObject() failed");      
    }            
    JsonObject& info = root["query"]["results"]["channel"]["item"];
    for(int i =0; i<7; i++){
      display.clear();
      display.setCursor(0, 0);            
      JsonObject& info = root["query"]["results"]["channel"]["item"]["forecast"][i];
      String date = info["date"];
      String day = info["day"];
      float high = info["high"];
      float low = info["low"];
      String w = info["text"];
      
      display.println(day + " - " + date );
      display.println("");
      display.println("High: " + String(FtoC(high)) + "\nLow: " + String(FtoC(low)));
      display.println();
      display.println(w);
      display.update();
      delay(3000);
    }
    

  }
}



float FtoC(float F){
  return ((F-32)*5)/9;
}

