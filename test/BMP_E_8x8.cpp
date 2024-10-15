
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
#include <Wire.h>               // Only needed for Arduino 1.6.5 and earlier
#include <LiquidCrystal_I2C.h>  // HD44780 steuert LiquidCrystal I2C
#include <Adafruit_Sensor.h>    // allgemein fuer Adafruitumgebung noetig
#include <DHT.h>                // Temperatursensor
#include <DHT_U.h>              // Temperatursensor
#include <MD_MAX72xx.h>         // LED Display



// BMP280_DEV - I2C Communications, Default Configuration, Normal Conversion
/////////////////////////////////////////////////////////////////////////////////

#include <BMP280_DEV.h>                           // Include the BMP280_DEV.h library

float temperature, pressure, altitude;            // Create the temperature, pressure and altitude variables
BMP280_DEV bmp280;                                // Instantiate (create) a BMP280_DEV object and set-up for I2C operation (address 0x77)


// Variablendeklaration

float t1;
float h1;
float absHum;
int h;
int t;
int i = 0;

// See guide for details on sensor wiring and usage:
// https://learn.adafruit.com/dht/overview

#define DHTPIN 5     // Digital pin connected to the DHT sensor 
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

// Uncomment the type of sensor in use:
// #define DHTTYPE    DHT11     // DHT 11
#define DHTTYPE    DHT22     // DHT 22 (AM2302)
// #define DHTTYPE    DHT21     // DHT 21 (AM2301)


DHT_Unified dht(DHTPIN, DHTTYPE);

// invertUpperZone = (HARDWARE_TYPE == MD_MAX72XX::GENERIC_HW || HARDWARE_TYPE == MD_MAX72XX::PAROLA_HW);
// Man kann drehen spiegeln und so weiter
// Treiber fuer 8x8 LED Anzeige
#define HARDWARE_TYPE MD_MAX72XX::PAROLA_HW
#define MAX_DEVICES  1 //Anzahl der gesteckten Displays
// auch 5, 18,19 Pin CSO Clk MisO ev. moeglich
#define CLK_PIN   14   // PIN Display 8x8   /12
#define DATA_PIN  13   // PIN Display 8x8  /11
#define CS_PIN    12   // PIN Display 8x8 /10

//Adresse und Zeilen fuer LCD
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

//Set Display 8x8
MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);



// Funktionsdeklaration

void PointSet()
  {
    mx.clear();
    mx.setPoint( t , h , true);    
  }

void PointSetu()
  {
    mx.clear();

    mx.setPoint( i , i , true);  
  }

void relais()  {
  if (t1 < 20)
  {
  delay(2000);
  digitalWrite(25 , HIGH);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Heizung an");
  delay(2000);
  }
  else
  { 
  delay(2000);
  lcd.clear();
  lcd.setCursor(2,0);
  digitalWrite(25 , LOW);
  lcd.print("Es ist warm !!!");
  lcd.setCursor(3,1);
  lcd.print("GENUG !!");
  delay(2000);
  }
}

void humidity()  {
  if (h1 > 75){
  delay(2000);
  digitalWrite(25 , HIGH);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Fenster auf");
  delay(2000);
  }
  else
  { 
  delay(2000);
  lcd.clear();
  lcd.setCursor(0,0);
  digitalWrite(25 , LOW);
  lcd.print("Fenster zu");
  delay(2000);
  }
}


void setup() {

// BMP 280 
  bmp280.begin(BMP280_I2C_ALT_ADDR);              //(BMP280_I2C_ALT_ADDR) Default initialisation with alternative I2C address (0x76), place the BMP280 into SLEEP_MODE 
  //  bmp280.setPresOversampling(OVERSAMPLING_X4);    // Set the pressure oversampling to X4
  //  bmp280.setTempOversampling(OVERSAMPLING_X1);    // Set the temperature oversampling to X1
  //  bmp280.setIIRFilter(IIR_FILTER_4);              // Set the IIR filter to setting 4
  //  bmp280.setTimeStandby(TIME_STANDBY_2000MS);     // Set the standby time to 2 seconds
  //  bmp280.startNormalConversion();                 // Start BMP280 continuous conversion in NORMAL_MODE




  // 9,10,11 ohne IN,OUT sowie 34,35,36,39 nur IN sowie 2 jedoch IN, OUT aber funktioniert nicht
  // 21, 22 und 16, 17 sind fuer I2C verwendbar
  // 1, 3 sind fuer RX und TX vorgesehen nicht IN-OUT benutzbar
  // 0 ist nicht fuer IN und OUT vorgesehen, funktionieren aber
  // 9, 10, 11 DO NOT USE ?????  liegen aber dauerhaft 3,3 Volt an
   // fuer LCD
  
  pinMode( 25 , OUTPUT);
  digitalWrite(25 , LOW);

  pinMode( 25 , OUTPUT);

  digitalWrite(25 , LOW);

  delay(500);

  lcd.init();                      // initialize the lcd 
  
  lcd.backlight();                 // Backlight on
  

  //u8g2.begin(); 
  Serial.begin(9600);
  // initialize the Matrix, sollte nur einmal aufgerufen werden
  mx.begin();
  // Initialize device.
  dht.begin();
  Serial.println("DHTxx Unified Sensor Example");
  // Print temperature sensor details.
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
 
  // Print humidity sensor details.
  dht.humidity().getSensor(&sensor);
 
  // Aufruf Funktion PointSetu()
  PointSetu();
  
}


void loop() {
  
 
  // Delay between measurements.
  delay(2000);
// Get temperature event and print its value.
  sensors_event_t event;
  t1 =dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println("Error reading temperature!");
  }
  else {
    Serial.println();
    Serial.println("Werte fuer Temperatur und Rel.Feuchtigkeit");
    Serial.print("Temperature: ");
    Serial.print(event.temperature);
    t1 = event.temperature;
    Serial.println();
  }
  // Get humidity event and print its value.
    dht.humidity().getEvent(&event);
    if (isnan(event.relative_humidity)) {
    Serial.println("Error reading humidity!");
  }
  else {
    Serial.print("Humidity: ");
    Serial.print(event.relative_humidity);
    Serial.println("%");
    h1 = event.relative_humidity;
    Serial.println();

  }
  
  //  printValues();

  if (bmp280.getMeasurements(temperature, pressure, altitude))    // Check if the measurement is complete
  {
    Serial.print(temperature);                    // Display the results    
    Serial.print("*C   ");
    Serial.print(pressure);    
    Serial.print("hPa   ");
    Serial.print(altitude);
    Serial.println("m");  
  }

  // Berechnung absulute Feuchtigkeit
  absHum =( 6.112 * exp((17.67*t1)/(t1+243.5))*h1*2.1674)/((273.15+t1)*1.239); // 1.239 ist Umrechnung m3/kg nach g/kg
  t = map(t1, 18, 26, 0,7);  // Anzahl der Displaypunkte 8 ( 0 -7 )
  h = map(absHum, 7, 20, 0,7);  // Anzahl der Displaypunkte 8 ( 0 -7 )
  
  delay(2000);
  PointSet();       // Aufruf der oben definierten Funktion

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Humi/% = ");
  lcd.print(h1);
  lcd.setCursor(0,1);
  lcd.print("Tem/Grad = ");
  lcd.print(t1);
  delay(2000);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("absHumi = ");
  lcd.print(absHum);
  lcd.setCursor(0,1);
  lcd.print("Tem/Grad = ");
  lcd.print(t1);
  
  delay(2000);
  digitalWrite(25 , LOW);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Humimap = ");
  lcd.print(h+1);
  lcd.setCursor(0,1);
  lcd.print("Temmap = ");
  lcd.print(t+1);
  
  delay(2000);
  // Beschreibung
  Serial.print("maphum ");
  Serial.print(h);
  Serial.println(" LED");

  Serial.print("maptemp ");
  Serial.print(t);
  Serial.println(" LED");

  // Aufrufen der obigen Funktionen
  relais();
  delay(2000);
  humidity();
  delay(2000);
}