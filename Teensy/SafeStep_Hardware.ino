/*
This is the main .ino file to be used with the Safe Step senior design project.

Authors are:
    Joseph Coker
    Emily Miller
    Colby Stevens
    Ryan Harper
*/

#include "Adafruit_Sensor.h"
#include "Adafruit_AM2320.h"

Adafruit_AM2320 am2320 = Adafruit_AM2320();

//initializing sensor pins
int sound_sensor = A2;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    delay(10); // hang out until serial port opens
  }

  am2320.begin();
}

void loop() {
  //temp_humidity();
  decibel_sensor();
}

void temp_humidity(){
  float f_temp = ((am2320.readTemperature()) * (1.8)) + 32;
  Serial.print("Temp: "); Serial.print(f_temp); Serial.println(" F");
  Serial.print("Hum: "); Serial.print(am2320.readHumidity()); Serial.println(" %RH");

  delay(2000);
}

void decibel_sensor(){
  float decibel_level = 20 * log10(analogRead(sound_sensor));
  Serial.println(analogRead(sound_sensor));
  delay(500);
}
