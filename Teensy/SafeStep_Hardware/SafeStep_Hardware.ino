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
#include <string>


// THERMISTOR SETUP
// which analog pin to connect
#define THERMISTORPIN A14      
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

//Temp & Humidity Sensor
Adafruit_AM2320 am2320 = Adafruit_AM2320(&Wire2);

//Decibel Sensor
int test_sensor = A16;

//GPS
SoftwareSerial GPSSerial(8,7);
Adafruit_GPS GPS(&GPSSerial);
#define GPSECHO true
uint32_t timer = millis();

//Accelerometer
Adafruit_MMA8451 mma = Adafruit_MMA8451();//&Wire1

void setup() {  
  //Begin serial communication with Arduino and Arduino IDE (Serial Monitor)
  Serial.begin(9600);
  //Begin serial communication with Arduino and HC-05
  mySerial.begin(9600);
  //Startup temp & humidity sensor
  am2320.begin();
  //Startup up GPS
  GPSSerial.begin(9600);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
  GPS.sendCommand(PGCMD_ANTENNA);

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
  Serial.println(i);
  i += 1;



  //Serial.print("The code is running \n");
  //fall_detection();
  //temp_humidity();
  //ThermTemp();
  //GPS_data();
  //decibel_read()

  //make the string to send
  String send = "Fall: " + fall_detection() + ",Temp:"+ ThermTemp() + ", " +GPS_data()+", Noise: "+decibel_read();
  
  sendBTData(send);
  delay(1500);
}

String temp_humidity(){
  double f_temp = ((am2320.readTemperature()) * (1.8)) + 32;
  double hum_RH = am2320.readHumidity();
  Serial.print("Temp: "); Serial.print(f_temp); Serial.println(" F");
  Serial.print("Hum: "); Serial.print(hum_RH); Serial.println(" %RH");
  double heat_index = calculateHeatIndex(f_temp, hum_RH);
  Serial.print("Heat Index: ");Serial.println(heat_index);
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
String decibel_read(){
  float decibel_level = 20 * log10(analogRead(test_sensor));
  Serial.println(decibel_level);
  return conversion(decibel_level);
/*
  if (decibel_level>=60) {
    Serial.println("Caution: Loud Noise");
    return "Caution: Loud Noise";
  }
  else {
    Serial.println("Safe level of Noise");
    return "WORKING";
  }
  */
  
}

String GPS_data(){
  char c = GPS.read();
  // if you want to debug, this is a good time to do it!
  if ((c) && (GPSECHO))
    Serial.write(c);

  // if a sentence is received, we can check the checksum, parse it...
  if (GPS.newNMEAreceived()) {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences!
    // so be very wary if using OUTPUT_ALLDATA and trytng to print out data
    //Serial.println(GPS.lastNMEA());   // this also sets the newNMEAreceived() flag to false

    if (!GPS.parse(GPS.lastNMEA()))   // this also sets the newNMEAreceived() flag to false
      return;  // we can fail to parse a sentence in which case we should just wait for another
  }

  // approximately every 2 seconds or so, print out the current stats
  if (millis() - timer > 2000) {
    timer = millis(); // reset the timer

    Serial.print("GPS Fix: "); Serial.print((int)GPS.fix);
    Serial.print(" quality: "); Serial.println((int)GPS.fixquality);
    if (GPS.fix) {
      Serial.print("Location: ");
      Serial.print(GPS.latitude, 4); Serial.print(GPS.lat);
      Serial.print(", ");
      Serial.print(GPS.longitude, 4); Serial.println(GPS.lon);

      Serial.print("Speed (knots): "); Serial.println(GPS.speed);
      Serial.print("Angle: "); Serial.println(GPS.angle);
      Serial.print("Altitude: "); Serial.println(GPS.altitude);
      Serial.print("Satellites: "); Serial.println((int)GPS.satellites);
      Serial.print("Antenna status: "); Serial.println((int)GPS.antenna);
    }
  }
  return String("GPS Fix: " + String((int)GPS.fix));
}

 
String fall_detection(){
  
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
  Serial.println("Acceleration: " + String(acceleration,2));
  if (acceleration > 15.0) {
    Serial.println("Fall Detected");
    return "Fall detected!";
    // Add your fall detection logic or alert mechanism here
  }else{
    Serial.println("No Fall Detected");
    return "No Fall Detected";
  }

}

String conversion(double value){
  return String(value, 2);
}

int sendBTData(String message){
  Serial.print("BT Message:" + message);
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
  
  Serial.println("Temperature:" + conversion(faren));
  return conversion(faren);
}