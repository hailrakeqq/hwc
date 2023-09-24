/*
lcd: 
d1 - scl
d2 - sda
*/

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <FS.h>
#include <Wire.h>
#include <SPI.h>
#include <vector>
#include <string>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "config.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define CHANGE_STATE_BTN D5
#define GO_TO_NEXT_SENSOR_BTN D6

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const String apiToken = API_TOKEN;

boolean isHardwareMonitor = false;
boolean isWeatherMonitor = true;
int sensorCount = 0;
int currentSensor = 0;

struct WeatherData
{
  const char *name;
  const char *country;
  const char *localtime;
  float temp_c;
  float feelslike_c;
};

struct Sensor
{
  String name;
  float temp;
  uint16_t fanSpeed;
  float ppt;
};

std::vector<Sensor> sensors;

void setup()
{
  Serial.begin(115200);
  WiFi.begin(SSID, PASSWORD);

  pinMode(GO_TO_NEXT_SENSOR_BTN, INPUT_PULLUP);
  pinMode(CHANGE_STATE_BTN, INPUT_PULLUP);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // 0x3C is the I2C address of the display
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  Serial.println("Connected to WiFi");
  display.clearDisplay();
  display.display();
}

void drawHardwareDisplay(Sensor &sensor){
  if(currentSensor >= sensorCount)
    currentSensor = 0;
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print(sensor.name);
  display.setCursor(90, 0);
  display.print('(');
  display.print(currentSensor + 1);
  display.print("/");
  display.print(sensorCount);
  display.print(')');
  display.print('\n');
  display.print("Temp: ");
  display.print(sensor.temp);
  display.print('\n');
  display.print("PPT: ");
  display.print(sensor.ppt); 
  display.print('\n');
  display.print("Fan Speed: ");
  display.print(sensor.fanSpeed);
  display.display();
}

void drawNoSensorDisplay(){
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Sensors not found");
}

void drawWeatherDisplay(const WeatherData& weather){
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);

    display.print(weather.name);
    display.print(", ");
    display.println(weather.country);

    display.println(weather.localtime);

    display.print("Temperature: ");
    display.print(weather.temp_c);
    display.println("C");

    display.print("Feels Like: ");
    display.print(weather.feelslike_c);
    display.println("C");

    display.display();
}

WeatherData GetWeatherData() {
  WeatherData weather;
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    WiFiClient wifiClient;
    DynamicJsonDocument doc(8192);

    http.begin(wifiClient,"http://api.weatherapi.com/v1/current.json?key=" + apiToken +"&q=Kyiv&aqi=no");

    int httpResponseCode = http.GET();

    if (httpResponseCode == HTTP_CODE_OK) {
      String response = http.getString();

      DeserializationError error = deserializeJson(doc, response);
      if(error){
        Serial.print("Deserialization error: ");
        Serial.println(error.c_str());
        return weather;
      }

      weather.name = doc["location"]["name"];
      weather.country = doc["location"]["country"];
      weather.localtime = doc["location"]["localtime"];
      weather.temp_c = doc["current"]["temp_c"];
      weather.feelslike_c = doc["current"]["feelslike_c"];

      http.end();

      return weather;
    } else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
      http.end();
    }
  }
  return weather;
}

void handleButtonsEvent() {
  static uint32_t btnTimer = 0;
  
  if (digitalRead(GO_TO_NEXT_SENSOR_BTN) == LOW && millis() - btnTimer > 100) {
    btnTimer = millis();
    if(isHardwareMonitor){
      if (currentSensor < sensorCount)
        currentSensor++;
      else
        currentSensor = 0;
    } else {
      return;
    }
  }
  if(digitalRead(CHANGE_STATE_BTN) == LOW && millis() - btnTimer > 100){
    btnTimer = millis();
    if(isWeatherMonitor) {
      isWeatherMonitor = false;
      isHardwareMonitor = true;
    } else {
      isWeatherMonitor = true;
      isHardwareMonitor = false;
    }
  }
}

const long buttonCheckInterval = 100; 
unsigned long previousButtonMillis = 0;
unsigned long lastApiRequestTime = 0;
const unsigned long apiRequestInterval = 60000; 
bool isApiRequestPending = false; 

std::vector<Sensor> getParsedSensorVector(String input){
    std::vector<Sensor> sensors;
    int startPos = 0;
    int endPos = input.indexOf('|', startPos);

    while (endPos != -1)
    {
        Sensor sensor;
        String sensorData = input.substring(startPos, endPos);
        int semicolonPos = sensorData.indexOf(';');

        if (semicolonPos != -1)
        {
            sensor.name = sensorData.substring(0, semicolonPos);
            sensorData.remove(0, semicolonPos + 1);

            sensor.temp = sensorData.toFloat();
            semicolonPos = sensorData.indexOf(';');

            if (semicolonPos != -1)
            {
                sensorData.remove(0, semicolonPos + 1);
                sensor.fanSpeed = sensorData.toInt();
                semicolonPos = sensorData.indexOf(';');

                if (semicolonPos != -1)
                {
                    sensorData.remove(0, semicolonPos + 1);
                    sensor.ppt = sensorData.toFloat();
                    sensors.push_back(sensor);
                }
            }
        }

        startPos = endPos + 1;
        endPos = input.indexOf('|', startPos);
    }

    return sensors;
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousButtonMillis >= buttonCheckInterval) {
    previousButtonMillis = currentMillis;
    handleButtonsEvent();
  }

  if (currentMillis - lastApiRequestTime >= apiRequestInterval) {
    isApiRequestPending = true;
    lastApiRequestTime = currentMillis; 
  }

  if (isWeatherMonitor) {
    if(isApiRequestPending){
      drawWeatherDisplay(GetWeatherData());
      isApiRequestPending = false;
    }
  } else if (isHardwareMonitor) {
    if(Serial.available()) {
      String incomingString = Serial.readStringUntil('\n'); 
      char str[incomingString.length() + 1];

      incomingString.toCharArray(str, sizeof(str));

      Serial.write(str);

      sensors = getParsedSensorVector(str);
      sensorCount = sensors.capacity();
    }
  
    if(!sensors.empty())
      drawHardwareDisplay(sensors[currentSensor]);
    else
      drawNoSensorDisplay();
  }
}


