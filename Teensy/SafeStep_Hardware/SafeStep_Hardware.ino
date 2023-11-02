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


double calculateHeatIndex(double temperatureF, double humidity) {
  // Returns the Heat Index based on the temperature in Farenheight and Humidity
  // Rothfusz regression equation and the adjustments based on the provided information to calculate the heat index accurately.

    double T = temperatureF; // Temperature
    double RH = humidity; // Relative Humidity

    // Check if the temperature is within a valid range
    if (T < 80.0 || RH < 13.0) {
        return T; // Heat index is not calculated for low temperatures or humidity.
    }

    double HI;

    if (T >= 80.0 && T <= 112.0 && RH <= 85.0) {
        HI = -42.379 + 2.04901523 * T + 10.14333127 * RH
            - 0.22475541 * T * RH - 0.00683783 * T * T
            - 0.05481717 * RH * RH + 0.00122874 * T * T * RH
            + 0.00085282 * T * RH * RH - 0.00000199 * T * T * RH * RH;
    } else {
        HI = 0.5 * (T + 61.0 + ((T - 68.0) * 1.2) + (RH * 0.094));
    }

    // Check if an adjustment is needed
    if (RH < 13.0 && T >= 80.0 && T <= 112.0) {
        double adjustment = ((13.0 - RH) / 4.0) * sqrt((17.0 - fabs(T - 95.0)) / 17.0);
        HI -= adjustment;
    
    } else if (RH > 85.0 && T >= 80.0 && T <= 87.0) {
        double adjustment = ((RH - 85.0) / 10.0) * ((87.0 - T) / 5.0);
        HI += adjustment;
    }

    return HI;
}
