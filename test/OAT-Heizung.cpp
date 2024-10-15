
// Fuer DHT
// DHT Temperature & Humidity Sensor
// Unified Sensor Library Example
// Written by Tony DiCola for Adafruit Industries
// Released under an MIT license.

// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor

#include <Arduino.h>
// For a connection via I2C using the Arduino Wire include:
#include <Wire.h> // Only needed for Arduino 1.6.5 and earlier

#include <Adafruit_Sensor.h> // allgemein fuer Adafruitumgebung noetig
#include <DHT.h>             // Temperatursensor
#include <DHT_U.h>           // Temperatursensor

// FUer OTA noetig
#include <WiFi.h>             
#include <ESPmDNS.h>        // Gibt ev. per DNS IP Upload frei
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
// HD44780 steuert LiquidCrystal I2C
#include <LiquidCrystal_I2C.h> 
// WLAN SSID und Password
const char* host = "esp32";
const char* ssid = "5GH";
const char* password = "112330720040719440";
//#include <WiFiClient.h>
//#include <WebServer.h>
// BMP280_DEV - I2C Communications, Default Configuration, Normal Conversion
/////////////////////////////////////////////////////////////////////////////////

#include <BMP280_DEV.h> // Include the BMP280_DEV.h library

//Adresse und Zeilen fuer LCD
LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display                            // Instantiate (create) a BMP280_DEV object and set-up for I2C operation (address 0x77)

// Variablendeklaration

float t1;
float h1;

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





void setup()
{
  lcd.init(); // initialize the lcd
  lcd.backlight(); // Backlight on
  pinMode(2, OUTPUT);
  Serial.begin(9600);
  lcd.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("Not");
  }
  lcd.println("");
  lcd.print("Connected to ");
  lcd.println(ssid);
  lcd.print("IP address: ");
  lcd.println(WiFi.localIP());

  


  while (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
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
                         lcd.println("End Failed");
                     });
  ArduinoOTA.begin();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.println("Ready");
  lcd.setCursor(0, 1);
  lcd.print("IP");
  lcd.println(WiFi.localIP());
  delay(2000);



  // 9,10,11 ohne IN,OUT sowie 34,35,36,39 nur IN sowie 2 jedoch IN, OUT aber funktioniert nicht
  // 21, 22 und 16, 17 sind fuer I2C verwendbar
  // 1, 3 sind fuer RX und TX vorgesehen nicht IN-OUT benutzbar
  // 0 ist nicht fuer IN und OUT vorgesehen, funktionieren aber
  // 9, 10, 11 DO NOT USE ?????  liegen aber dauerhaft 3,3 Volt an
  // fuer LCD

  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);

  //u8g2.begin();
  Serial.begin(9600);
  // initialize the Matrix, sollte nur einmal aufgerufen werden

  dht.begin();
  lcd.println("DHTxx Unified Sensor Example");
  // Print temperature sensor details.
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);

  // Print humidity sensor details.
  dht.humidity().getSensor(&sensor);

  // Aufruf Funktion PointSetu()
}

void loop()
{


  ArduinoOTA.handle();



  // Get temperature event and print its value.
  sensors_event_t event;
  t1 = dht.temperature().getEvent(&event);
  if (isnan(event.temperature))
  {
    lcd.println("Error reading temperature!");
  }
  else
  {
    Serial.println();
    Serial.println("Werte fuer Temperatur und Rel.Feuchtigkeit");
    Serial.print("Temperature: ");
    Serial.print(event.temperature);
    t1 = event.temperature;
    Serial.println();
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity))
  {
    lcd.println("Error reading humidity!");
  }
  else
  {
    Serial.print("Humidity: ");
    Serial.print(event.relative_humidity);
    Serial.println("%");
    h1 = event.relative_humidity;
    Serial.println();
  }
  if (t1 > 24)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Heizung aus");
    lcd.setCursor(0, 1);
    lcd.print("Temp : ");
    lcd.print(t1);
    digitalWrite(2, LOW);
    delay(2000);
  }
  else
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Die Heizung an");
    lcd.setCursor(0, 1);
    lcd.print("Temp : ");
    lcd.print(t1);
    digitalWrite(2, HIGH);
    delay(2000);
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Humi/% = ");
  lcd.print(h1);
  lcd.setCursor(0, 1);
  lcd.print("Tem/Grad = ");
  lcd.print(t1);
  
  delay(2000);

  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("IP-Adresse:");
  lcd.setCursor(2, 1);
  lcd.println(WiFi.localIP());

  delay(2000);

 

}
