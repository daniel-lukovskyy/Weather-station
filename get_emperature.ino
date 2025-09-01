#include <WiFi.h>
#include <WiFiUdp.h>
#include <HTTPClient.h>
#include <NTPClient.h>
#include <ArduinoJson.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1); // &Wire is link for I^2 object ,-1 is res port

const char* SSID = "YOUR WiFi";
const char* PASSWORD = "YOUR PASSWORD";

String API_key = "YOUR API";
String city = "YOUR CITY";
String url = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "&appid=" + API_key + "&units=metric";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP,"pool.ntp.org", 3 * 3600, 60000);

void setup() {

  Serial.begin(115200);
  delay(1000);
  Serial.print("Connecting to WiFi...");
  WiFi.begin(SSID,PASSWORD);

  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print("...");
  }
  Serial.println(WiFi.status());
  timeClient.begin();
  
}

void loop() {
  if (WiFi.status() == WL_CONNECTED){
    HTTPClient http;
    http.begin(url);
    timeClient.update();

    
    int http_code =http.GET(); //gets JSON,HTML or text

    if (http_code > 0){
      String payload =http.getString(); //saving body of the answer as string
      Serial.println(payload);

      StaticJsonDocument<4096> doc; // making space for payload, 4096 is max
      DeserializationError error = deserializeJson(doc, payload); // reads payload, saves in doc

      //error_raise
      if (error){
        Serial.print("deserializeJson failed: " + String(error.f_str())); //.fstr returns text disc of error
        return;
      }

      int hours = timeClient.getHours();
      int minutes = timeClient.getMinutes();
      

      //getting data
      float temperature = doc["main"]["temp"];

      Serial.println("\n --Parsed Data--");
      Serial.println("Temperature: " + String(temperature) + "C Time: " + String(hours) + ":" + String(minutes) + ":");
    }

    http.end();
  }
  delay(60000);
}
