
// Fuer DHT
// DHT Temperature & Humidity Sensor
// treiber fuer zwei CO2 Sensoren MHZ19 und SCD30
// ESP8266 MQ135 CO2 MQ7 MQ6 MQ5 MQ4 Luft Air Gas CO2
// Unified Sensor Library Example
// Written by Guenter Pruefer
// Released under ..........

// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor
// - Wire
// - <WiFi.h>
// - <ESPmDNS.h> // Gibt ev. per DNS IP Upload frei
// - <WiFiUdp.h>
// - <ArduinoOTA.h> Update per Wlan
// - <LiquidCrystal_I2C.h> fuer Display over I2C
// - <WiFiUdp.h>
// - "time.h"
// - <ctime>
// - <stdio.h>
// - <iostream>
// - <chrono>
// - <SPI.h>
// - "SparkFun_SCD30_Arduino_Library.h" CO2 Sensor Click here to get the library: http://librarymanager/All#SparkFun_SCD30

// For a connection via I2C using the Arduino Wire include:

#include <Adafruit_Sensor.h> // allgemein fuer Adafruitumgebung noetig
#include <DHT.h>             // Temperatursensor
#include <DHT_U.h>           // Temperatursensor
#include <SPI.h>
// FUer OTA noetig
#include <WiFi.h>
#include <ESPmDNS.h> // Gibt ev. per DNS IP Upload frei
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
// HD44780 steuert LiquidCrystal I2C
#include <LiquidCrystal_I2C.h>
// WLAN SSID und Password

// Zeitserver
#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <TZ.h>
// using ssid_char_t = const char;
#else // ESP32
#include <WiFi.h>
// using ssid_char_t = char;
#endif
#include <ESPPerfectTime.h>
#include <Wire.h>

//#include "SparkFun_SCD30_Arduino_Library.h" //Click here to get the library: http://librarymanager/All#SparkFun_SCD30
// SCD30 airSensor;
#include <WiFi.h>
#include <WiFiUdp.h>
#include "time.h"
#include <ctime>
#include <stdio.h>
#include <iostream>
#include <chrono>
#include <Arduino.h>
const char *host = "esp32";
const char *ssid = "5GH";
const char *password = "112330720040719440";

// Adresse und Zeilen fuer LCD
LiquidCrystal_I2C lcd(0x27, 20, 4); // set the LCD address to 0x27 for a 16 chars and 2 line display                            // Instantiate (create) a BMP280_DEV object and set-up for I2C operation (address 0x77)

// Variablendeklaration

float temperatur;  // Temperatur Messwert DHT 22
float huminity;    // Feuchte rel. Messwert DHT 22
float temp = 25; // Sollwert Raumtemperatur
float COx = 0;
float voltage = 0;
float tAusNacht = 19.00;
float tOnTag = 5.00;
float CO2 = 0;

// See guide for details on sensor wiring and usage:
// https://learn.adafruit.com/dht/overview

#define DHTPIN 5 // Digital pin connected to the DHT sensor
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

// Uncomment the type of sensor in use:
// #define DHTTYPE    DHT11     // DHT 11
#define DHTTYPE DHT22 // DHT 22 (AM2302)
// #define DHTTYPE    DHT21     // DHT 21 (AM2301)

DHT_Unified dht(DHTPIN, DHTTYPE);
#define analogPin 34

void printLocalTime()
{
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo))
  {
    Serial.println("Failed to obtain time");
    return;
  }
}

void setup()
{
  pinMode(analogPin, INPUT_PULLDOWN);
  analogRead(analogPin);

  // airSensor.begin(); // SCD 30 CO2 Sensor
  lcd.init(); // initialize the lcd

  lcd.backlight(); // Backlight on
  pinMode(2, OUTPUT);
  Serial.begin(9600);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.println("Booting now ");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print("Not");
    Wire1.begin(); // Start the wire hardware that may be supported by your platform
  }
  // NTP-Zeit-Server
  const char *ntpServer = "ptbtime1.ptb.de";
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.println("NTP: DE");
  lcd.setCursor(0, 1);
  lcd.print("Connected to: ");
  lcd.println(ssid);
  lcd.setCursor(0, 2);
  lcd.print("IP address: ");
  lcd.setCursor(0, 3);
  lcd.println(WiFi.localIP());

  //  t3  = airSensor.getCO2();
  //  Serial.println(t3);

  while (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  // ArduinoOTA.setHostname("myesp8266");

  // No authentication by default
  // ArduinoOTA.setPassword((const char *)"123");

  ArduinoOTA.onStart([]()
                     { lcd.println("Start"); });
  ArduinoOTA.onEnd([]()
                   { lcd.println("\nEnd"); });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total)
                        { lcd.printf("Progress: %u%%\r", (progress / (total / 100))); });
  ArduinoOTA.onError([](ota_error_t error)
                     {
                       lcd.clear();
                       lcd.setCursor(0, 0);
                       lcd.printf("Error[%u]: ", error);
                       if (error == OTA_AUTH_ERROR)
                         lcd.println("Auth Failed");
                       else if (error == OTA_BEGIN_ERROR)
                         lcd.println("Begin Failed");
                       else if (error == OTA_CONNECT_ERROR)
                         lcd.println("Connect Failed");
                       else if (error == OTA_RECEIVE_ERROR)
                         lcd.println("Receive Failed");
                       else if (error == OTA_END_ERROR)
                         lcd.println("End Failed"); });

  ArduinoOTA.begin();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.println("Ready");
  lcd.setCursor(0, 1);
  lcd.print("IP");
  lcd.setCursor(0, 2);
  lcd.println(WiFi.localIP());
  delay(2000);

  // 9,10,11 ohne IN,OUT sowie 34,35,36,39 nur IN sowie 2 jedoch IN, OUT aber funktioniert nicht
  // 21, 22 und 16, 17 sind fuer I2C verwendbar
  // 1, 3 sind fuer RX und TX vorgesehen nicht IN-OUT benutzbar
  // 0 ist nicht fuer IN und OUT vorgesehen, funktionieren aber
  // 9, 10, 11 DO NOT USE ?????  liegen aber dauerhaft 3,3 Volt an
  // fuer LCD

  pinMode(32, OUTPUT);
  pinMode(33, OUTPUT);
  pinMode(2, OUTPUT);
  digitalWrite(32, LOW);
  digitalWrite(33, LOW);
  digitalWrite(2, LOW);

  Serial.begin(9600);
  // initialize the Matrix, sollte nur einmal aufgerufen werden

  dht.begin();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.println("DHTxx Unified Sensor Example");
  // Print temperature sensor details.
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);

  // Print humidity sensor details.
  dht.humidity().getSensor(&sensor);

  // Aufruf Funktion PointSetu()

  // Zeitserver
  // Configure SNTP client in the same way as built-in one
#ifdef ESP8266
  pftime::configTzTime(TZ_Asia_Tokyo, ntpServer);
#else // ESP32
  pftime::configTzTime(PSTR("JST-1"), ntpServer);
#endif
}

void loop()
{
  ArduinoOTA.handle();
  // Pruefen ob WiFi connect
  while (WiFi.waitForConnectResult() != WL_CONNECTED)
  {

    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  // Aufruf Funktion printlocalTime
  printLocalTime();

  // stuct tm nach tsstruct
  time_t now = time(0);
  struct tm tstruct = *localtime(&now);

  // aus tsstruct std , min, sek und alle als float
  // fuer auswerten Tag - Nacht Schaltung
  float std = tstruct.tm_hour;
  float mini = tstruct.tm_min;
  float min = mini / 60;
  float sek = tstruct.tm_sec;
  sek = sek / 3600;
  float alle = std + min + sek;
  // fuer Darstellung auf Display
  int st = std;
  int mi = mini;

  // Ausgabe Serial
  Serial.println(std);

  Serial.println(min);
  Serial.println(sek);
  Serial.print("Zeit Ausgabe dezimal:  ");
  Serial.print(alle);
  Serial.println("  in Stunden");
  Serial.println();

  // Get temperature event and print its value DHT22.
  sensors_event_t event;
  temperatur = dht.temperature().getEvent(&event);
  if (isnan(event.temperature))
  {

    Serial.println("Error reading temperature!");
  }
  else
  {
    Serial.println();
    Serial.println("Werte fuer Temperatur und Rel.Feuchtigkeit");
    Serial.print("Temperature: ºC");
    Serial.print(event.temperature);
    temperatur = event.temperature;
    Serial.println();
  }
  // Get humidity event and print its value DHT22.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity))
  {
    Serial.println("Error reading humidity!");
  }
  else
  {
    Serial.print("Humidity: ");
    Serial.print(event.relative_humidity);
    Serial.println("%");
    huminity = event.relative_humidity;
    Serial.println();
  }

  if (((temperatur < temp)) && ((alle > tOnTag) && (alle < tAusNacht)))
  {
    // Die Temperatur ist unter dem Sollwert und die Zeit ist Nachtzeit
    lcd.clear();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Die Heizung an");
    lcd.setCursor(0, 1);
    lcd.print("Temp/ Grad: ");
    lcd.print(temperatur);
    lcd.setCursor(0, 2);
    lcd.print("Sollwert/ Grad");
    lcd.setCursor(0, 3);
    lcd.print(temp);
    digitalWrite(32, HIGH);
    digitalWrite(33, HIGH);
    digitalWrite(2, HIGH);
    delay(30000);
  }

  else

  {
    if ((alle > tOnTag) && (alle < tAusNacht))
    {
      // Die Zeit ist am Tag aber die Temperatur ist ueber dem Sollwert
      lcd.clear();
      lcd.backlight();
      lcd.setCursor(0, 0);
      lcd.print("Heizung aus");
      lcd.setCursor(0, 1);
      lcd.print("Temp Grad: ");
      lcd.print(temperatur);
      lcd.setCursor(0, 2);
      lcd.print("Sollwert Grad: ");
      lcd.setCursor(0, 3);
      lcd.print(temp);
      digitalWrite(32, LOW);
      digitalWrite(33, LOW);
      digitalWrite(2, LOW);
      delay(4000);
    }
    else
    {
      lcd.clear();
      lcd.noBacklight();
      // Die Zeit ist in der Nacht
      lcd.setCursor(0, 0);
      lcd.print("Nacht aus");
      lcd.setCursor(0, 1);
      lcd.print(tAusNacht);
      lcd.setCursor(6, 1);
      lcd.print("-");
      lcd.setCursor(9, 1);
      lcd.print(tOnTag);
      lcd.setCursor(15, 1);
      lcd.print("Uhr");
      lcd.setCursor(0, 2);
      lcd.print("Temp: Grad ");
      lcd.print(temperatur);
      lcd.setCursor(0, 3);
      lcd.print("Solltemeratur:Grad ");
      lcd.print(temp);
      digitalWrite(32, LOW);
      digitalWrite(33, LOW);
      digitalWrite(2, LOW);
      delay(4000);
    }
  }

  ArduinoOTA.handle();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Nacht-in :");
  lcd.setCursor(12, 0);
  lcd.println(tAusNacht);
  lcd.setCursor(0, 1);
  lcd.print("Tag-on : ");
  lcd.print(tOnTag);
  lcd.setCursor(0, 2);
  lcd.print("Temp-soll");
  lcd.print(temp);
  lcd.setCursor(0, 3);
  lcd.print("Temp-In: ");
  lcd.print(temperatur);
  delay(4000);

  ArduinoOTA.handle();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("aktuelle Zeit: ");
  lcd.setCursor(0, 1);
  lcd.printf("%02d:%02d\n", st, mi);
  lcd.setCursor(5, 1);
  lcd.print("  Uhr");
  lcd.setCursor(0, 2);
  lcd.print("Humanitity");
  lcd.setCursor(0, 3);
  lcd.print(huminity);

  // delay(2000);
  pinMode(analogPin, INPUT_PULLDOWN);
  delay(2000);
  voltage = analogRead(analogPin);
  voltage = voltage;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Aromat (ppm):");
  lcd.print((voltage + (temperatur * 0.0106 * voltage) / 100)*2.5); // 500-600 ppm normal
  lcd.setCursor(0, 1);
  lcd.print("max. 1200 ppm ");
  lcd.setCursor(0, 2);
  lcd.print("CO2(ppm):");
  lcd.print(CO2 + 1500);
  lcd.setCursor(0, 3);
  lcd.print("max. 2500 ppm ");
  delay(2000);

  ArduinoOTA.handle();
  delay(2000);

  digitalWrite(33, HIGH);
  delay(1000);

  digitalWrite(33, LOW);
  delay(1000);

  digitalWrite(33, HIGH);
  delay(1000);

  ArduinoOTA.handle();
  delay(1000);

  digitalWrite(33, HIGH);
  delay(1000);

  digitalWrite(33, LOW);
  delay(1000);

  digitalWrite(33, HIGH);
  delay(1000);

  ArduinoOTA.handle();
  delay(1000);

  digitalWrite(33, HIGH);
  delay(1000);

  digitalWrite(33, LOW);
  delay(1000);

  digitalWrite(33, HIGH);
  delay(1000);

   digitalWrite(33, HIGH);
  delay(1000);

  digitalWrite(33, LOW);
  delay(1000);

  digitalWrite(33, HIGH);
  delay(1000);

  ArduinoOTA.handle();
  delay(1000);

  digitalWrite(33, HIGH);
  delay(1000);

  digitalWrite(33, LOW);
  delay(1000);

  digitalWrite(33, HIGH);
  delay(1000);

  ArduinoOTA.handle();
  delay(1000);

  digitalWrite(33, HIGH);
  delay(1000);

  digitalWrite(33, LOW);
  delay(1000);

  digitalWrite(33, HIGH);
  delay(1000);

   digitalWrite(33, HIGH);
  delay(1000);

  digitalWrite(33, LOW);
  delay(1000);

  digitalWrite(33, HIGH);
  delay(1000);

  ArduinoOTA.handle();
  delay(1000);

  digitalWrite(33, HIGH);
  delay(1000);

  digitalWrite(33, LOW);
  delay(1000);

  digitalWrite(33, HIGH);
  delay(1000);

  ArduinoOTA.handle();
  delay(1000);

  digitalWrite(33, HIGH);
  delay(1000);

  digitalWrite(33, LOW);
  delay(1000);

  digitalWrite(33, HIGH);
  delay(1000);

  ArduinoOTA.handle();
  delay(1000);

  digitalWrite(33, HIGH);
  delay(1000);

  digitalWrite(33, LOW);
  delay(1000);

  digitalWrite(33, HIGH);
  delay(1000);

  ArduinoOTA.handle();
  delay(1000);

  digitalWrite(33, HIGH);
  delay(1000);

  digitalWrite(33, LOW);
  delay(1000);

  digitalWrite(33, HIGH);
  delay(1000);

  ArduinoOTA.handle();
  delay(1000);

  digitalWrite(33, HIGH);
  delay(1000);

  digitalWrite(33, LOW);
  delay(1000);

  digitalWrite(33, HIGH);
  delay(1000);

   digitalWrite(33, HIGH);
  delay(1000);

  digitalWrite(33, LOW);
  delay(1000);

  digitalWrite(33, HIGH);
  delay(1000);

  ArduinoOTA.handle();
  delay(1000);

  digitalWrite(33, HIGH);
  delay(1000);

  digitalWrite(33, LOW);
  delay(1000);

  digitalWrite(33, HIGH);
  delay(1000);

  ArduinoOTA.handle();
  delay(1000);

  digitalWrite(33, HIGH);
  delay(1000);

  digitalWrite(33, LOW);
  delay(1000);

  digitalWrite(33, HIGH);
  delay(1000);

   digitalWrite(33, HIGH);
  delay(1000);

  digitalWrite(33, LOW);
  delay(1000);

  digitalWrite(33, HIGH);
  delay(1000);

  ArduinoOTA.handle();
  delay(1000);

  digitalWrite(33, HIGH);
  delay(1000);

  digitalWrite(33, LOW);
  delay(1000);

  digitalWrite(33, HIGH);
  delay(1000);

  ArduinoOTA.handle();
  delay(1000);

  digitalWrite(33, HIGH);
  delay(1000);

  digitalWrite(33, LOW);
  delay(1000);

  delay(3000);
  
}
