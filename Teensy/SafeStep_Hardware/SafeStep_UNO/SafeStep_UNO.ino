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
#include <SoftwareSerial.h>
//#include <string>


// THERMISTOR SETUP
// which analog pin to connect
#define THERMISTORPIN A0   
// resistance at 25 degrees C
#define THERMISTORNOMINAL 10000      
// temp. for nominal resistance (almost always 25 C)
#define TEMPERATURENOMINAL 25   
// how many samples to take and average, more takes longer
// but is more 'smooth'
#define NUMSAMPLES 5
// The beta coefficient of the thermistor (usually 3000-4000)
#define BCOEFFICIENT 3950
// the value of the 'other' resistor
#define SERIESRESISTOR 10000    

int samples[NUMSAMPLES];



//INITIALIZING SENSOR PINS
//Bluetooth Sensor (HC=05)
SoftwareSerial mySerial(0, 1); //HC-05 Tx & Rx is connected to Arduino #3 & #2


//Decibel Sensor
int test_sensor = A1;

//Accelerometer
Adafruit_MMA8451 mma = Adafruit_MMA8451();//&Wire1

void setup() {  
  //Begin serial communication with Arduino and Arduino IDE (Serial Monitor)
  Serial.begin(9600);
  //Begin serial communication with Arduino and HC-05

  //Startup accelerometer  
  if (! mma.begin()) {
    Serial.println("Couldnt start");
    while (1);
  }
  mma.setRange(MMA8451_RANGE_2_G);

  Serial.println("Setting up ...");
}

int i = 0;
void loop() {
  //Serial.println(i);
  //i += 1;



  //Serial.print("The code is running \n");
  //fall_detection();
  //temp_humidity();
  //ThermTemp();
  //GPS_data();
  //decibel_read()

  //make the string to send
  String send = "Fall: " + fall_detection() + ",Temp:"+ ThermTemp() + ",Noise: "+decibel_read();
  
  sendBTData(send);
  delay(1500);
}


//used to test the decibel cap which will turn the digital output to HIGH
String decibel_read(){
  float decibel_level = 20 * log10(analogRead(test_sensor));
  //Serial.println(decibel_level);
  return conversion(decibel_level);
  
}

 
String fall_detection(){
  
  /* Get a new sensor event */ 
  sensors_event_t event; 
  mma.getEvent(&event);
  /* Display the results (acceleration is measured in m/s^2) */
  //Serial.print("X: \t"); Serial.print(event.acceleration.x); Serial.print("\t");
  //Serial.print("Y: \t"); Serial.print(event.acceleration.y); Serial.print("\t");
  //Serial.print("Z: \t"); Serial.print(event.acceleration.z); Serial.print("\t");
  //Serial.println("m/s^2 ");
  
  // Calculate the magnitude of acceleration
  float acceleration = sqrt(event.acceleration.x * event.acceleration.x +
       event.acceleration.y * event.acceleration.y +
       event.acceleration.z * event.acceleration.z);

  // Check for a fall (you may need to adjust the threshold)
  //Serial.println("Acceleration: " + String(acceleration,2));
  if (acceleration > 15.0) {
    //Serial.println("Fall Detected");
    return "Fall detected!";
    // Add your fall detection logic or alert mechanism here
  }else{
    //Serial.println("No Fall Detected");
    return "No Fall Detected";
  }

}

String conversion(double value){
  return String(value, 2);
}

int sendBTData(String message){
  Serial.print("\n"+message);
  mySerial.print("\n" + message);

  return 0;
}

String ThermTemp(){
  uint8_t i;
  float average;

  // take N samples in a row, with a slight delay
  for (i=0; i< NUMSAMPLES; i++) {
   samples[i] = analogRead(THERMISTORPIN);
   delay(10);
  }
  
  // average all the samples out
  average = 0;
  for (i=0; i< NUMSAMPLES; i++) {
     average += samples[i];
       //Serial.print("analog reading "); 
      //Serial.println(samples[i]);
  }
  average /= NUMSAMPLES;

  //Serial.print("Average analog reading "); 
  //Serial.println(average);
  
  // convert the value to resistance
  average = 1023 / (average - 1);
  average = SERIESRESISTOR / average;
  //Serial.print("Thermistor resistance "); 
  //Serial.println(average);
  
  float steinhart;
  steinhart = average / THERMISTORNOMINAL;     // (R/Ro)
  steinhart = log(steinhart);                  // ln(R/Ro)
  steinhart /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
  steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
  steinhart = 1.0 / steinhart;                 // Invert
  steinhart -= 273.15;                         // convert absolute temp to C
  
  float faren = steinhart * (9/5) + 32;
  
  //Serial.println("Temperature:" + conversion(faren));
  return conversion(faren);
}