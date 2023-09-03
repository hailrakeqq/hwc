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

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "config.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define TEST_LED 15 // it exist only for test it should be deleted
#define CHANGE_STATE_BTN 14
#define GO_TO_NEXT_SENSOR_BTN 12

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const String apiToken = API_TOKEN;

boolean isHardwareMonitor = false;
boolean isWeatherMonitor = true;
int sensorCount = 3;
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
  char* name;
  float temp;
  uint16_t fanSpeed;
  float ppt;
};

Sensor moqSensor[3];

void initializeSensor(Sensor& sensor, const char* name, float fanSpeed, float ppt, float temp) {
    strncpy(sensor.name, name, sizeof(sensor.name) - 1);
    sensor.fanSpeed = fanSpeed;
    sensor.ppt = ppt;
    sensor.temp = temp;
}

void setup()
{
  Serial.begin(115200);
  WiFi.begin(SSID, PASSWORD);

  // initializeSensor(moqSensor[0], "sensor1", 1150, 35.2, 39.9);
  // initializeSensor(moqSensor[1], "sensor2", 1200, 36.5, 38.0);
  // initializeSensor(moqSensor[2], "sensor3", 1100, 34.8, 40.5);
  
  pinMode(TEST_LED, OUTPUT);
  pinMode(14, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);

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
}

void drawHardwareDisplay(Sensor &sensors){
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);

    display.print(sensors.name);
    display.print('\n');
    display.print("Temp: ");
    display.print(sensors.temp);
    display.print('\n');

    display.print("PPT: ");
    display.print(sensors.ppt); 
    display.print('\n');

    display.print("Fan Speed: ");
    display.print(sensors.fanSpeed);

    display.display();
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

    http.begin(wifiClient,"http://api.weatherapi.com/v1/current.json?key=" + apiToken +"&q=Kiev&aqi=no");

    int httpResponseCode = http.GET();

    if (httpResponseCode == HTTP_CODE_OK) {
      String response = http.getString();
      Serial.println(response);

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
  uint32_t btnTimer = 0;
  
  if (digitalRead(GO_TO_NEXT_SENSOR_BTN) == LOW && millis() - btnTimer > 100) {
    btnTimer = millis();
    if (currentSensor < sensorCount)
      currentSensor++;
    else
      currentSensor = 0;
}
  if(digitalRead(CHANGE_STATE_BTN) == LOW && millis() - btnTimer > 100){
    btnTimer = millis();
    if(isWeatherMonitor){
      isWeatherMonitor = false;
      isHardwareMonitor = true;
      Serial.println("hardware monitor on");
    }
    else
    {
      isWeatherMonitor = true;
      isHardwareMonitor = false;
      Serial.println("Weather monitor on");
    }
  }
}

unsigned long previousMillis = 0;
const long interval = 60000; 
const long buttonCheckInterval = 100; 
unsigned long previousButtonMillis = 0;

void loop() {
  unsigned long currentMillis = millis();
  unsigned long currentButtonMillis = millis();

  if (currentButtonMillis - previousButtonMillis >= buttonCheckInterval) {
    previousButtonMillis = currentButtonMillis;
    handleButtonsEvent();
  }

  if (isWeatherMonitor) {
    digitalWrite(TEST_LED, LOW);
    if (currentMillis - previousMillis >= interval) {
      drawWeatherDisplay(GetWeatherData());
      previousMillis = currentMillis;
    }
  }
  if(isHardwareMonitor){
    digitalWrite(TEST_LED, HIGH);
    if (currentMillis - previousMillis >= interval) {
      drawHardwareDisplay(moqSensor[currentSensor]);
      previousMillis = currentMillis;
    }
  }
}