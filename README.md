# Weather-station
Portable weather station based on ESP32 microcontroller.
The station uses data from "https://openweathermap.org/" , using JSON.
Notice: You have to log in and get your API before use.

Additionally station works with DHT11 sensor wich measures temperature and humidity in real time. 
Notice : Using DHT11 sensor allows you to take measurments every 2 seconds. If you need more often you have to use DHT22 sensor.
Notice : If you use only sensor and not the whole module, you have to add 10kOmh resistor between DATA and VCC.

Usage of capacitor of 0.1 microFarad ensures you that data from sensor won't lost and the signal won't be interrupted.

#What does this do ?
The weather station shows ont OLED ddisplay the temperature and humidity at sensors location. By pressing the button you can switch between data from the sensor and data from the weather web site. You can choose any city that you want.
By making some small changes in the code you can adjust the weather data that you will see. (see discription of the code).
