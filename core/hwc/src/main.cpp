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
  char name[25];
  float temp;
  uint16_t fanSpeed;
  float ppt;
};

//TODO: доделать что бы можно было переключаться между елементами массива структур
//TODO: Связать железяку с пк что бы оно автоматически отправляло данные, а микроконтроллер их получал

void setup()
{
  SPIFFS.begin();
  Serial.begin(115200);
  WiFi.begin(SSID, PASSWORD);

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
  SPIFFS.remove("/data.txt");
  Serial.println("Connected to WiFi");
}

void drawHardwareDisplay(Sensor &sensors){
  if(currentSensor >= sensorCount){
    currentSensor = 0;
  } else {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);

    display.print(sensors.name);

    display.setCursor(90, 0);
    display.print('(');
    display.print(currentSensor + 1);
    display.print("/");
    display.print(sensorCount);
    display.print(')');
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

//TODO: режими змінюються повільно, потрібно задебажити щоб зрозуміти де саме працює повільно
String data = "";

void loop(){
#pragma region test
if (Serial.available()) {
  String incomingString = "";
  while (Serial.available()) {
    incomingString = Serial.readStringUntil('\n'); // Читаем символ из COM-порта
    // data += incomingString; // Добавляем символ к строке
  }

  // char *result = (char *)malloc(data.length());
  // data.toCharArray(result, data.length());
  char *result = (char *)malloc(incomingString.length());
  incomingString.toCharArray(result, incomingString.length());
  Serial.write(result);
  Serial.write('\n');
  free(result);

  // char *arr = (char *)malloc(100);
  // String receivedData = Serial.readStringUntil('\n'); // Чтение строки до символа '\n'

  // Serial.write("Received data: ");
  // receivedData.toCharArray(arr, 100);
  // Serial.write(arr); // Вывод считанных данных в Serial Monitor
  // free(arr);
}
#pragma endregion

  // struct Sensor receivedSensor;
  // char buffer[sizeof(Sensor)];
  // if (Serial.available() >= sizeof(Sensor))
  // {
  //   // Прочитать данные из COM-порта и сохранить их в структуру
  //    Serial.readBytes(buffer, sizeof(Sensor));
    
  //   // Вывести содержимое буфера на экран
  //   Serial.write(buffer, sizeof(Sensor));

  //   // Serial.readBytes((char *)&receivedSensor, sizeof(Sensor));
  //   // drawHardwareDisplay(receivedSensor);
  // }
#pragma region mainWork
  // unsigned long currentMillis = millis();

  // if (currentMillis - previousButtonMillis >= buttonCheckInterval) {
  //   previousButtonMillis = currentMillis;
  //   handleButtonsEvent();
  // }

  // if (currentMillis - previousMillis >= interval) {
  //   previousMillis = currentMillis;
    
  //   if (isWeatherMonitor) {
  //     digitalWrite(TEST_LED, LOW);
  //     drawWeatherDisplay(GetWeatherData());
  //   } else if (isHardwareMonitor) {
  //     digitalWrite(TEST_LED, HIGH);
  //     Serial.print("===============\nBegin drawing display\n==================\n");
  //     drawHardwareDisplay(moqSensor[currentSensor]);
  //     Serial.print("===============\nEnd drawing display\n==================\n");
  //   }
  // }
#pragma endregion
}