#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid =     "my ssid";       //This is where you put in your home network's SSID to connect to
const char* password = "my password";   //This is where you put in your home network's password

const String url = "http://api.coindesk.com/v1/bpi/currentprice.json";
unsigned long next_refresh = 0;

HTTPClient http;

void setup() {
  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.print("Connecting to '");
  Serial.print(ssid);
  Serial.print("' ");
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print(" connected!");

  Serial.print("My IP is: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if( (long)(millis() - next_refresh) >= 0)
  {
    http.begin(url);
    int code = http.GET();
    
    if (code == 200) {
      String payload = http.getString();
  
      DynamicJsonBuffer jsonBuffer(1100);
      JsonObject& root = jsonBuffer.parseObject(payload);
      JsonObject& bpi = root["bpi"];
      JsonObject& bpi_EUR = bpi["EUR"];
      int last = bpi_EUR["rate_float"];  
      
      String sSSID = "📈 1 bitcoin = € ";
      sSSID += last;
      sSSID += ",-";
      
      WiFi.softAP(sSSID.c_str());
    } else {
      Serial.print("Failed to request coindesk API - is the internet connection active? Return code: ");
      Serial.println(code);
      
      WiFi.softAP("😥 Help me I'm broken!");
    }
    
    http.end();
    next_refresh = millis() + 30000;
  }
}
