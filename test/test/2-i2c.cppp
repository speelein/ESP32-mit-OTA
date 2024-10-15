TwoWire I2Cone = TwoWire( 0 );
TwoWire I2Ctwo = TwoWire( 1 );

 void  setup () {
 I2Cone. beginnen ( 21 , 22 , 100000 );  //  SDA pin 21, SCL pin 22, 100kHz Frequenz 
 I2Ctwo. begin ( 16 , 17 , 400000 );  //  SDA pin 16, SCL-pin 17, 400kHz Frequenz 
}

 void  loop () {
 I2Cone. beginTransmission ( 0x38 );
 I2Cone. write ( 0x00  |  0x80 );
 I2Cone. write ( 0x03 );
 I2Cone. endTransmission ();

 I2Ctwo. beginTransmission ( 0x42 );
 I2Ctwo. write ( 140 );
 I2Ctwo. endTransmission ();
 I2Ctwo. requestFrom ( 0x42 , 1 );
  wenn  (I2Ctwo. verfügbar () ==  1 ) {
 byte Wert = I2Ctwo. read ();
}
} 


2.Beispiel


/* I2C slave Address Scanner
Kutscher07: Modified for TTGO TQ board with builtin OLED
Quelle: https://github.com/espressif/arduino-esp32/issues/977
*/

#include <Wire.h>

#define SDA1 21
#define SCL1 22

#define SDA2 17
#define SCL2 16

TwoWire I2Cone = TwoWire(0);
TwoWire I2Ctwo = TwoWire(1);

void scan1(){
Serial.println("Scanning I2C Addresses Channel 1");
uint8_t cnt=0;
for(uint8_t i=0;i<128;i++){
  I2Cone.beginTransmission(i);
  uint8_t ec=I2Cone.endTransmission(true);
  if(ec==0){
    if(i<16)Serial.print('0');
    Serial.print(i,HEX);
    cnt++;
  }
  else Serial.print("..");
  Serial.print(' ');
  if ((i&0x0f)==0x0f)Serial.println();
  }
Serial.print("Scan Completed, ");
Serial.print(cnt);
Serial.println(" I2C Devices found.");
}

void scan2(){
Serial.println("Scanning I2C Addresses Channel 2");
uint8_t cnt=0;
for(uint8_t i=0;i<128;i++){
  I2Ctwo.beginTransmission(i);
  uint8_t ec=I2Ctwo.endTransmission(true);
  if(ec==0){
    if(i<16)Serial.print('0');
    Serial.print(i,HEX);
    cnt++;
  }
  else Serial.print("..");
  Serial.print(' ');
  if ((i&0x0f)==0x0f)Serial.println();
  }
Serial.print("Scan Completed, ");
Serial.print(cnt);
Serial.println(" I2C Devices found.");
}

void setup(){
Serial.begin(115200);
I2Cone.begin(SDA1,SCL1,400000); // SDA pin 21, SCL pin 22 TTGO TQ
I2Ctwo.begin(SDA2,SCL2,400000); // SDA2 pin 17, SCL2 pin 16 
}

void loop(){
scan1();
Serial.println();
delay(100);
scan2();
Serial.println();
delay(5000);
} 





Beispiel 3

/* I2C slave Address Scanner
for 5V bus
 * Connect a 4.7k resistor between SDA and Vcc
 * Connect a 4.7k resistor between SCL and Vcc
for 3.3V bus
 * Connect a 2.4k resistor between SDA and Vcc
 * Connect a 2.4k resistor between SCL and Vcc
Kutscher07: Modified for TTGO TQ board with builtin OLED
 */

#include <Wire.h>

#include <dummy.h>  //for esp32

#define SDA1 21
#define SCL1 22

#define SDA2 5
#define SCL2 4


TwoWire I2Cone = TwoWire(0);
TwoWire I2Ctwo = TwoWire(1);

void scan1(){
Serial.println("Scanning I2C Addresses Channel 1");
uint8_t cnt=0;
for(uint8_t i=0;i<128;i++){
  I2Cone.beginTransmission(i);
  uint8_t ec=I2Cone.endTransmission(true);
  if(ec==0){
    if(i<16)Serial.print('0');
    Serial.print(i,HEX);
    cnt++;
  }
  else Serial.print("..");
  Serial.print(' ');
  if ((i&0x0f)==0x0f)Serial.println();
  }
Serial.print("Scan Completed, ");
Serial.print(cnt);
Serial.println(" I2C Devices found.");

}
void scan2(){
Serial.println("Scanning I2C Addresses Channel 2");
uint8_t cnt=0;
for(uint8_t i=0;i<128;i++){
  I2Ctwo.beginTransmission(i);
  uint8_t ec=I2Ctwo.endTransmission(true);
  if(ec==0){
    if(i<16)Serial.print('0');
    Serial.print(i,HEX);
    cnt++;
  }
  else Serial.print("..");
  Serial.print(' ');
  if ((i&0x0f)==0x0f)Serial.println();
  }
Serial.print("Scan Completed, ");
Serial.print(cnt);
Serial.println(" I2C Devices found.");

}
void setup(){
Serial.begin(115200);
I2Cone.begin(SDA1,SCL1,400000); // SDA pin 21, SCL pin 22 TTGO TQ
I2Ctwo.begin(SDA2,SCL2,400000); // SDA pin 5, SCL pin 4 builtin OLED

}
void loop(){
scan1();
Serial.println();
delay(100);
scan2();
Serial.println();
delay(5000);

}


UV - VEML 6070

#include <Wire.h>
#include "Adafruit_VEML6070.h"

Adafruit_VEML6070 uv = Adafruit_VEML6070();

void setup()
{
Serial.begin(9600);
Serial.println("VEML6070 Test");
uv.beginnen(VEML6070_1_T); // pass in the integration time constant
}

void loop()
{
Serial.print("UV-Licht Ebene: ");
Serial.println(uv.readUV());
delay(1000);
}