#include <WiFi.h>
#include <HTTPClient.h>

const char* WIFI_SSID = "YOUR WiFi";
const char* WIFI_PASS = "YOUR PASSWORD";

String apiKey = "YOUR API";
String city = "YOUR CITY";
String url = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "&appid=" + apiKey + "&units=metric";

void setup() {

  Serial.begin(115200);
  delay(1000);

  Serial.println("Connecting to WiFi...");
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.println("...");
  }
  Serial.println("WiFi is connected");

}

void loop() {
  if(WiFi.status() == WL_CONNECTED){
    HTTPClient http;
    http.begin(url);

    Serial.println("Requesting for : " + url);
    int httpCode = http.GET();

    if(httpCode > 0){
      Serial.print("HTTP response code: ");
      Serial.println(httpCode);

      String payload = http.getString();
      Serial.println("Server response: " );
      Serial.println(payload);
  

      int pos = payload.indexOf("\"temp\":");
      if (pos != -1){
        int end = payload.indexOf(",", pos);
        String temp = payload.substring(pos + 7 ,end);
        Serial.print("\nTemperature in ");
        Serial.print(city);
        Serial.print(": ");
        Serial.print(temp);
        Serial.println(" °C");
      }

    } else{
      Serial.print("HTTP request error, code: ");
      Serial.println(httpCode);
    }
    delay(10000);

  }
}
