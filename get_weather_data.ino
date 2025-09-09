#include <WiFi.h>
#include <WiFiUdp.h>
#include <HTTPClient.h>
#include <NTPClient.h>
#include <ArduinoJson.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DHT.h"

#define pin_button 18
#define dht_pin 4
#define DHT_TYPE DHT11

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

unsigned long last_update = 0;
unsigned long lst_read = 0;

char buf1[32], buf2[32], bufcity[32]; //buffers for weather

DHT dht(dht_pin, DHT_TYPE);

bool city_weather = false;
int last_button_state = HIGH;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1); // &Wire is link for I^2 object ,-1 is res port

const char* SSID = "//Your WiFi name";
const char* PASSWORD = "//Your WiFi password";

String API_key = "Your API key";
String city = "Your City";
String url = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "&appid=" + API_key + "&units=metric";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP,"pool.ntp.org", 3 * 3600, 60000);

void setup() {

  pinMode(pin_button, INPUT_PULLUP);

  dht.begin();

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

  Wire.begin(21,22);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    while(true); //if adress not found, 0X3C is adress
  }
  display.clearDisplay();
  Serial.println("Display OK");
  
}


void loop() {
  
  int button_state = digitalRead(pin_button);

  if (button_state == LOW && last_button_state == HIGH){
    delay(50);
    if(digitalRead(pin_button) == LOW){
      city_weather = !city_weather;
    }
  }
  last_button_state = button_state;

  if (city_weather){

    unsigned long now = millis();
    if (now - last_update > 60000 || last_update == 0){
      if (WiFi.status() == WL_CONNECTED){
        HTTPClient http;
        http.begin(url);
        timeClient.update();

    
        int http_code =http.GET(); //gets JSON,HTML or text

        if (http_code > 0){
          String payload =http.getString(); //saving body of the answer as string
          //Serial.println(payload); // unmute if needed

          StaticJsonDocument<2048> doc; // making space for payload, 4096 is max
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
          float humidity = doc["main"]["humidity"];

          //Serial.println("\n --Parsed Data--");
          //Serial.println("Temperature: " + String(temperature) + "C Time: " + String(hours) + ":" + String(minutes) + ":"); //unmute if needed
          snprintf(buf1, sizeof(buf1), "Temperature %.1f C", temperature);
          snprintf(buf2, sizeof(buf2), "Humidity %.1f%%", humidity);
          snprintf(bufcity, sizeof(bufcity), "Location: %s", city.c_str()); // .c_str() makes const char* , otherwise snprintf won't work
          
        
        }
      http.end();
      }
    last_update = now;
    }

    const char* data[] = {bufcity, buf1, buf2};
    int len_data = sizeof(data) / sizeof(data[0]);
    center_txt(data, len_data, 1);

  }else{

    if(millis() - lst_read > 5000){
      lst_read = millis();

      char tempr_buf[32], humid_buf[32], location_buf[32];
      float temperature_dht = dht.readTemperature();
      float humidity_dht = dht.readHumidity();

      //check sensor
      if (isnan(temperature_dht) || isnan(humidity_dht)) {
        char check_buf[32];
        const char* err_msg = "No sensor data";
        snprintf(check_buf, sizeof(check_buf), "%s", err_msg);
        const char* err_sens[] = {check_buf};
        int len_err_sens = sizeof(err_sens) / sizeof(err_sens[0]);
        center_txt(err_sens, len_err_sens, 1);
      }else {
        const char* location = "Current Location";
        snprintf(tempr_buf, sizeof(tempr_buf), "Temperature %.1f C", temperature_dht);
        snprintf(humid_buf, sizeof(humid_buf), "Humidity %.1f%%", humidity_dht);
        snprintf(location_buf, sizeof(location_buf), "%s" ,location);

        const char* data_sens[] = {location_buf ,tempr_buf, humid_buf};
        int len_data_sens = sizeof(data_sens) / sizeof(data_sens[0]);
        center_txt(data_sens, len_data_sens, 1);
        for (int i = 0; i < len_data_sens; i++) {Serial.println(data_sens[i]);}
      }
    }
  }
  delay(100);
}


