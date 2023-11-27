/*
This is the main .ino file to be used with the Safe Step senior design project.

Authors are:
    Joseph Coker
    Emily Miller
    Colby Stevens
    Ryan Harper
*/
#include <Wire.h>
#include <Adafruit_MMA8451.h>
#include "Adafruit_Sensor.h"
#include "Adafruit_AM2320.h"
#include <Adafruit_GPS.h>
#include <stdio.h>
#include <string>



//INITIALIZING SENSOR PINS
//Bluetooth Sensor (HC=05)
#define mySerial Serial1 //HC-05 Tx & Rx is connected to Arduino #3 & #2

//Temp & Humidity Sensor
Adafruit_AM2320 am2320 = Adafruit_AM2320(&Wire2);

//Decibel Sensor
int test_sensor = A16;

//GPS
#define GPSSerial Serial2
Adafruit_GPS GPS(&GPSSerial);

//Accelerometer
Adafruit_MMA8451 mma = Adafruit_MMA8451(&Wire1);

void setup() {  
  //Begin serial communication with Arduino and Arduino IDE (Serial Monitor)
  Serial.begin(9600);
  //Begin serial communication with Arduino and HC-05
  mySerial.begin(9600);
  //Startup temp & humidity sensor
  am2320.begin();
  //Startup up GPS
  GPSSerial.begin(9600);
  //Startup accelerometer  
  mma.begin();
  mma.setRange(MMA8451_RANGE_2_G);
}

void loop() {
  //Serial.print("The code is running \n");
  fall_detection();
  //temp_humidity();
  sendBTData(test_decibel());
  GPS_data();
  test_decibel();
}

String temp_humidity(){
  double f_temp = ((am2320.readTemperature()) * (1.8)) + 32;
  double hum_RH = am2320.readHumidity();
  Serial.print("Temp: "); Serial.print(f_temp); Serial.println(" F");
  Serial.print("Hum: "); Serial.print(hum_RH); Serial.println(" %RH");
  double heat_index = calculateHeatIndex(f_temp, hum_RH);
  Serial.print("Heat Index: ");Serial.println(heat_index);
  delay(2000);
  heat_index = round(heat_index * 1000)/1000;
   return conversion(heat_index);
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

//used to test the decibel cap which will turn the digital output to HIGH
String test_decibel(){
  float decibel_level = 20 * log10(analogRead(test_sensor));
  String audio = decibel_sensor(decibel_level);
  delay(1000);
  return audio;
}

String decibel_sensor(double val){
  
  Serial.println(val);
  // sensor detects a signal above the threshold value
  if (val>=60) {
    Serial.println("Caution: Loud Noise");
    return "Caution: Loud Noise";
  }
  else {
    Serial.println("Safe level of Noise");
    return "";
  }
}

double GPS_data(){
  if (Serial.available()) {
    char c = Serial.read();
    GPSSerial.write(c);
  }
  if (GPSSerial.available()) {
    char c = GPSSerial.read();
    Serial.println(c);
  }
  return 0;
}

 
String fall_detection(){
  // Read the 'raw' data in 14-bit counts
  mma.read();
  
  //Serial.println("I'm in fall_detection");
  Serial.print("X:\t"); Serial.print(mma.x); 
  Serial.print("\tY:\t"); Serial.print(mma.y); 
  Serial.print("\tZ:\t"); Serial.print(mma.z); 
  Serial.println();
  
  /* Get a new sensor event */ 
  sensors_event_t event; 
  mma.getEvent(&event);
  /* Display the results (acceleration is measured in m/s^2) */
  Serial.print("X: \t"); Serial.print(event.acceleration.x); Serial.print("\t");
  Serial.print("Y: \t"); Serial.print(event.acceleration.y); Serial.print("\t");
  Serial.print("Z: \t"); Serial.print(event.acceleration.z); Serial.print("\t");
  Serial.println("m/s^2 ");
  
  // Calculate the magnitude of acceleration
  float acceleration = sqrt(event.acceleration.x * event.acceleration.x +
       event.acceleration.y * event.acceleration.y +
       event.acceleration.z * event.acceleration.z);

  // Check for a fall (you may need to adjust the threshold)
  if (acceleration > 10.0) {
    return"Fall detected!";
    // Add your fall detection logic or alert mechanism here
  }else{
    return "";
  }

  delay(50);
}

String conversion(double value){
  return String(value, 2);
}

int sendBTData(String message){
  Serial.print(message);
  mySerial.print(message);
  delay(10);
  return 0;
}