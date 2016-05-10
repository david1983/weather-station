#include <Wire.h>
#include <SPI.h>
#include <ArduinoJson.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ssd1306syp.h>
#include <ESP8266WiFi.h>
#include "yahooWeather.h"
#include "asyncDelay.h"

static const unsigned char PROGMEM sunny[] ={B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000001, B00000000, B00000000, B00000000, B00000000, B00000000, B00000001, B00000000, B00000000, B00000000, B00000000, B00000000, B00000001, B00000000, B00000000, B00000000, B00000000, B00000000, B00000001, B00000000, B00000000, B00000000, B00000000, B00000000, B00000001, B00000000, B00000000, B00000000, B00000000, B00000000, B00000001, B00000000, B00000000, B00000000, B00000000, B01000000, B00000001, B00000000, B00000011, B00000000, B00000000, B01100000, B00000000, B00000000, B00000111, B00000000, B00000000, B00110000, B00000000, B00000000, B00001110, B00000000, B00000000, B00011000, B00000000, B00000000, B00011100, B00000000, B00000000, B00001100, B00000111, B11100000, B00011000, B00000000, B00000000, B00000000, B00011111, B11111000, B00000000, B00000000, B00000000, B00000000, B01110000, B00001110, B00000000, B00000000, B00000000, B00000000, B11100000, B00000011, B00000000, B00000000, B00000000, B00000001, B10000000, B00000001, B10000000, B00000000, B00000000, B00000001, B00000000, B00000000, B11000000, B00000000, B00000000, B00000011, B00000000, B00000000, B01000000, B00000000, B00000000, B00000110, B00000000, B00000000, B01100000, B00000000, B00000000, B00000110, B00000000, B00000000, B01100000, B00000000, B00000000, B00000110, B00000000, B00000000, B00100000, B00000000, B00000000, B00000100, B00000000, B00000000, B00110000, B00000000, B00000000, B00000100, B00000000, B00000000, B00110000, B00000000, B00111111, B10000100, B00000000, B00000000, B00110001, B11111100, B00000000, B00000100, B00000000, B00000000, B00100000, B00000000, B00000000, B00000110, B00000000, B00000000, B00100000, B00000000, B00000000, B00000110, B00000000, B00000000, B01100000, B00000000, B00000000, B00000011, B00000000, B00000000, B01100000, B00000000, B00000000, B00000011, B00000000, B00000000, B11000000, B00000000, B00000000, B00000001, B10000000, B00000001, B10000000, B00000000, B00000000, B00000000, B11000000, B00000011, B00000000, B00000000, B00000000, B00000000, B01110000, B00000110, B00000000, B00000000, B00000000, B00000000, B00111111, B11111100, B00000000, B00000000, B00000000, B00001100, B00001111, B11110000, B00010000, B00000000, B00000000, B00011100, B00000000, B00000000, B00011000, B00000000, B00000000, B00111000, B00000000, B00000000, B00001100, B00000000, B00000000, B01110000, B00000000, B00000000, B00000110, B00000000, B00000000, B01100000, B00000000, B00000000, B00000011, B00000000, B00000000, B00000000, B00000001, B00000000, B00000001, B00000000, B00000000, B00000000, B00000001, B00000000, B00000000, B00000000, B00000000, B00000000, B00000001, B00000000, B00000000, B00000000, B00000000, B00000000, B00000001, B00000000, B00000000, B00000000, B00000000, B00000000, B00000001, B00000000, B00000000, B00000000, B00000000, B00000000, B00000001, B00000000, B00000000, B00000000, B00000000, B00000000, B00000001, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000};
  static const unsigned char PROGMEM cloudy[] ={B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00110000, B00000000, B00000000, B00000000, B00000000, B00000000, B01110000, B00000000, B00000000, B00000000, B00000000, B00000000, B01111000, B00000000, B00000000, B00000000, B00000000, B00000000, B01111000, B00000000, B00000000, B00000000, B00000000, B00000000, B01111000, B00000000, B00000000, B00000000, B00000000, B00000000, B01111000, B00000000, B00000000, B00000000, B00000100, B00000000, B01110000, B00000000, B10000000, B00000000, B00001110, B00000000, B00110000, B00000011, B11000000, B00000000, B00011111, B00000000, B00000000, B00000111, B11000000, B00000000, B00001111, B10000000, B00000000, B00000111, B11000000, B00000000, B00001111, B10000000, B00000000, B00001111, B10000000, B00000000, B00000111, B11000000, B00000000, B00001111, B00000000, B00000000, B00000011, B10000001, B11111100, B00000110, B00000000, B00000000, B00000000, B00000111, B11111111, B00000000, B00000000, B00000000, B00000000, B00001111, B11111111, B11000000, B00000000, B00000000, B00000011, B00011111, B11111111, B11100000, B00000000, B00000000, B00111111, B11111111, B00000111, B11100000, B00000000, B00000000, B11111111, B11111110, B00000001, B11110000, B00000000, B00000001, B11111111, B11111110, B00000000, B11110000, B00000000, B00000111, B11111111, B11111111, B00000000, B11111000, B00000000, B00001111, B11100000, B00111111, B10000000, B01111000, B00000000, B00001111, B10000000, B00001111, B11000000, B01111000, B00111100, B00011111, B00000000, B00000111, B11000000, B01111000, B01111110, B00111110, B00000000, B00000011, B11111100, B01111000, B01111111, B00111100, B00000000, B00000001, B11111111, B11111000, B01111110, B00111100, B00000000, B00000001, B11111111, B11111000, B00111100, B01111000, B00000000, B00000000, B11111111, B11111000, B00000000, B01111000, B00000000, B00000000, B11111111, B11111000, B00000000, B01111000, B00000000, B00000000, B00000000, B11111000, B00000000, B01110000, B00000000, B00000000, B00000000, B01111100, B00000000, B01110000, B00000000, B00000000, B00000000, B00111100, B00000000, B01110000, B00000000, B00000000, B00000000, B00111110, B00000000, B01110000, B00000000, B00000000, B00000000, B00011110, B00000000, B01111000, B00000000, B00000000, B00000000, B00011110, B00000000, B01111000, B00000000, B00000000, B00000000, B00011110, B00000000, B01111000, B00000000, B00000000, B00000000, B00011110, B00000000, B01111100, B00000000, B00000000, B00000000, B00011110, B00000000, B00111100, B00000000, B00000000, B00000000, B00011110, B00000000, B00111110, B00000000, B00000000, B00000000, B00011110, B00000000, B00011110, B00000000, B00000000, B00000000, B00111100, B00000000, B00011111, B10000000, B00000000, B00000000, B01111100, B00000000, B00001111, B11000000, B00000000, B00000000, B11111100, B00000000, B00000111, B11111000, B00000000, B00000011, B11111000, B00000000, B00000011, B11111111, B11111111, B11111111, B11110000, B00000000, B00000001, B11111111, B11111111, B11111111, B11100000, B00000000, B00000000, B01111111, B11111111, B11111111, B11000000, B00000000, B00000000, B00011111, B11111111, B11111111, B00000000, B00000000};
  static const unsigned char PROGMEM rain[] ={B00000000, B00000011, B11111000, B00000000, B00000000, B00000000, B00000000, B00011111, B11111111, B00000000, B00000000, B00000000, B00000000, B01111111, B11111111, B11000000, B00000000, B00000000, B00000001, B11111111, B11111111, B11110000, B00000000, B00000000, B00000011, B11111111, B11111111, B11111000, B00000000, B00000000, B00000111, B11111100, B00000111, B11111100, B00000000, B00000000, B00001111, B11110000, B00000001, B11111110, B00000000, B00000000, B00011111, B11000000, B00000000, B01111111, B00000000, B00000000, B00011111, B10000000, B00000000, B00111111, B00000000, B00000000, B00111111, B00000000, B00000000, B00011111, B11111110, B00000000, B01111110, B00000000, B00000000, B00001111, B11111111, B10000000, B01111100, B00000000, B00000000, B00001111, B11111111, B11100000, B01111100, B00000000, B00000000, B00000111, B11111111, B11110000, B11111000, B00000000, B00000000, B00000111, B11001111, B11111000, B11111000, B00000000, B00000000, B00000000, B00000011, B11111000, B11111000, B00000000, B00000000, B00000000, B00000000, B11111100, B11111000, B00000000, B00000000, B00000000, B00000000, B01111110, B11111000, B00000000, B00000000, B00000000, B00000000, B00111110, B11111000, B00000000, B00000000, B00000000, B00000000, B00111110, B11111000, B00000000, B00000000, B00000000, B00000000, B00011111, B11111000, B00000000, B00000000, B00000000, B00000000, B00011111, B11111000, B00000000, B00000000, B00000000, B00000000, B00011111, B01111100, B00000011, B10000000, B00000001, B11000000, B00011111, B01111100, B00000111, B10000000, B00000011, B11100000, B00011111, B01111110, B00000111, B11000000, B00000011, B11100000, B00011111, B00111111, B00000111, B11000000, B00000011, B11100000, B00111110, B00111111, B00000111, B11000001, B10000011, B11100000, B00111110, B00011111, B10000111, B11000011, B11000011, B11100000, B01111110, B00001111, B10000111, B11000011, B11000011, B11100000, B11111100, B00000111, B10000111, B10000011, B11000001, B11100001, B11111000, B00000011, B10000001, B00000011, B11000000, B10000001, B11111000, B00000001, B10000000, B00000011, B11000000, B00000001, B11110000, B00000000, B00000000, B00000011, B11000000, B00000001, B11100000, B00000000, B00000000, B00000011, B11000000, B00000001, B10000000, B00000000, B00000000, B00000011, B10000000, B00000000, B00000000, B00000000, B00000011, B10000000, B00000001, B11000000, B00000000, B00000000, B00000111, B10000000, B00000011, B11100000, B00000000, B00000000, B00000111, B11000000, B00000011, B11100000, B00000000, B00000000, B00000111, B11000000, B00000011, B11100000, B00000000, B00000000, B00000111, B11000001, B10000011, B11100000, B00000000, B00000000, B00000111, B11000011, B11000011, B11100000, B00000000, B00000000, B00000111, B10000011, B11000011, B11100000, B00000000, B00000000, B00000111, B10000011, B11000001, B11000000, B00000000, B00000000, B00000001, B00000011, B11000000, B10000000, B00000000, B00000000, B00000000, B00000011, B11000000, B00000000, B00000000, B00000000, B00000000, B00000011, B11000000, B00000000, B00000000, B00000000, B00000000, B00000011, B11000000, B00000000, B00000000, B00000000, B00000000, B00000011, B10000000, B00000000, B00000000};
  static const unsigned char PROGMEM thunder[] ={B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000011, B11111100, B00000000, B00000000, B00000000, B00000000, B00000111, B11111111, B00000000, B00000000, B00000000, B00000000, B00011110, B00000011, B10000000, B00000000, B00000000, B00000000, B00111000, B00000001, B11000000, B00000000, B00000000, B00000000, B00110000, B00000000, B11100000, B00000000, B00000000, B00000000, B01100000, B00000000, B01100000, B00000000, B00000000, B00000000, B01100000, B00000000, B00110000, B00000000, B00000000, B00111111, B11000000, B00000000, B00111110, B00000000, B00000000, B11111111, B11000000, B00000000, B00011111, B10000000, B00000011, B11000000, B00000000, B00000000, B00000001, B11000000, B00000011, B00000000, B00000000, B00000000, B00000000, B11100000, B00000110, B00000000, B00000000, B00000000, B00000000, B01110000, B00001110, B00000000, B00000000, B00000000, B00000000, B00111000, B00001100, B00000000, B00000000, B00000000, B00000000, B00011000, B00011100, B00000000, B00000000, B00000000, B00000000, B00011000, B00011000, B00000000, B00000000, B00000000, B00000000, B00011000, B00011000, B00000000, B00000000, B00000000, B00000000, B00001100, B00011000, B00000000, B00000000, B00000000, B00000000, B00001100, B00011000, B00000000, B00000000, B00000000, B00000000, B00011000, B00011100, B00000000, B00000000, B00000000, B00000000, B00011000, B00001100, B00000000, B00000000, B00000000, B00000000, B00011000, B00001110, B00000000, B00000000, B00000000, B00000000, B00110000, B00000110, B00000000, B00000000, B00000000, B00000000, B01110000, B00000011, B00000000, B00000000, B00000000, B00000000, B11100000, B00000001, B11000000, B00000000, B00000000, B00000001, B11000000, B00000000, B11111111, B11111111, B11111111, B11111111, B10000000, B00000000, B00111111, B11111111, B11111111, B11111110, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00001000, B00000000, B00000000, B01000000, B00000000, B00000000, B00011100, B00001100, B00000001, B11000000, B00000000, B00000000, B00111000, B00011100, B00000011, B10000000, B00000000, B00000000, B01110000, B00111000, B00000111, B00000000, B00000000, B00000000, B11100000, B01110000, B00001110, B00000000, B00000000, B00000001, B11000000, B11100000, B00011100, B00000000, B00000000, B00000000, B00000001, B11111100, B00000000, B00000000, B00000000, B00000100, B00000000, B11111000, B01000000, B00000000, B00000000, B00001100, B00000000, B01110000, B11100000, B00000000, B00000000, B00011100, B00000000, B11100001, B11000000, B00000000, B00000000, B00010000, B00000001, B11000001, B10000000, B00000000, B00000000, B00000000, B00000011, B10000000, B00000000, B00000000, B00000000, B00000000, B00000011, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000};

const int iconSize = 48;

const int SDA_PIN=D1;
const int SCL_PIN=D2;

const char* ssid     = "ITap Group";
const char* password = "gameshooting1";

Adafruit_ssd1306syp display(SDA_PIN, SCL_PIN);

yahooWeather myW = yahooWeather();

void renderTitle(int x,int y){    
    display.clear();
    display.setCursor(x,y);
    display.println("Title");
    display.update();
}

void setup(){
    display.initialize();

    for(int i=0; i>0; i--){
      delay(5);
      renderTitle(0,i);
    }
    delay(1000);
    for(int i=0; i>-64; i--){
      delay(5);
      renderTitle(0,i);
    }
    WiFi.begin(ssid, password);
    Serial.begin(115200);
    while(WiFi.status()!=WL_CONNECTED){
      delay(500);
      Serial.print('.');
    }
    WiFiClient client;    
    Serial.println("connected");
    myW.Start(client,100);
    Serial.println("started");
}
int n=0;

struct view{ int y; String date; int temp; String text;String day; };

view currentView;

void getData(){
 if(n>5) n=0;
 String json = myW.getData();  
  DynamicJsonBuffer  jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(json);  
  if (!root.success()) {
      Serial.println("parseObject() failed");          
  } else{
    JsonObject& forecast = root["query"]["results"]["channel"]["item"]["forecast"][n];            
    view nextView;
    nextView.y = display.height();
    String date = forecast["date"];
    String text = forecast["text"];
    String day = forecast["day"];
    nextView.date = date;    
    nextView.text = text;
    nextView.day = day;
    handleView(nextView);
    n++;  
  }
}

void handleView(struct view newView){

  for(currentView.y; currentView.y > -display.height() ; currentView.y-=2){
    delay(5);
      renderV(currentView);  
  }
  for(newView.y; newView.y>0;newView.y-=2){    
    delay(5);
    renderV(newView);
  }
  currentView = newView;
}


void renderV(struct view v){  
  String s = v.text;
  s.toLowerCase();
  Serial.println(s + " : ");
  Serial.print(s.indexOf("rain"));
  

  display.clear();
  display.setCursor(0,v.y);
  display.setTextSize(1);
  display.println(v.date);
  display.println();
  display.setTextSize(2);
  display.println(v.day);
  display.println();
  if(s.indexOf("sun")>=0 || s.indexOf("sunny")>=0){
    display.drawBitmap(display.width()-iconSize,v.y+10,sunny,iconSize,iconSize,WHITE);  
  }else if(s.indexOf("cloudy")>=0 || s.indexOf("breezy")>=0 ){
    display.drawBitmap(display.width()-iconSize,v.y+10,cloudy,iconSize,iconSize,WHITE);  
  }else if(s.indexOf("rain")>=0){
    display.drawBitmap(display.width()-iconSize,v.y+10,rain,iconSize,iconSize,WHITE);  
  }else if(s.indexOf("thunderstorms")>=0){
    display.drawBitmap(display.width()-iconSize,v.y+10,thunder,iconSize,iconSize,WHITE);  
  }
  
  display.update();
}


void test(){
  Serial.println("test");
}

//asyncDelay a = asyncDelay(5000);
asyncDelay b = asyncDelay(5000);
void loop(){
  delay(500);  
  //a.runJob(&test);
  b.runJob(&getData);
}  

