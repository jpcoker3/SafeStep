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
//#include <SoftwareSerial.h>



//Create software serial object to communicate with HC-05 Bluetooth Module
#define mySerial Serial1 //HC-05 Tx & Rx is connected to Arduino #3 & #2


//INITIALIZING SENSOR PINS
//Temp & Humidity Sensor
Adafruit_AM2320 am2320 = Adafruit_AM2320(&Wire2);

//Decibel Sensor
int sound_sensor = A3;
boolean val = 0;

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


  //pinMode(sound_sensor, INPUT);
 // Serial.println("The setup has been done");
  //while (!Serial) {
    //delay(10); // hang out until serial port opens
  //}
  //setting up temp & humidity sensor
  am2320.begin();
  
  //setting up GPS
  GPSSerial.begin(115200);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
  GPS.sendCommand(PGCMD_ANTENNA);
  
  Serial.begin(9600);
  
  mma.begin();
  mma.setRange(MMA8451_RANGE_2_G);
  //Serial.print("Range = "); Serial.print(2 << mma.getRange());  
  //Serial.println("G");
}

void loop() {
  //Serial.print("The code is running \n");
  fall_detection();
  temp_humidity();
  sendBTData(temp_humidity());
  //sendBTData("<3 earl ");
  //decibel_sensor();
  //GPS_data();
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

//used to test the decibel cap which will turn the digital output to HIGH
void test_decibel(){
  int test_sensor = A3;
  float decibel_level = 20 * log10(analogRead(test_sensor));
  Serial.println(decibel_level);
  delay(500);
}

double decibel_sensor(){
  val =digitalRead(sound_sensor);
  Serial.println (val);
  // when the sensor detects a signal above the threshold value, LED flashes
  if (val==HIGH) {
    Serial.println("Caution: Loud Noise");
  }
  else {
    Serial.println("Safe level of Noise");
  }
 return 0;
}

double GPS_data(){
  Serial.println(GPS.read());
  if (GPS.newNMEAreceived()) {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences!
    // so be very wary if using OUTPUT_ALLDATA and trying to print out data
    Serial.print(GPS.lastNMEA()); // this also sets the newNMEAreceived() flag to false
    if (!GPS.parse(GPS.lastNMEA())) // this also sets the newNMEAreceived() flag to false
      return; // we can fail to parse a sentence in which case we should just wait for another
  }
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
  return 0.00;
}

String conversion(double value){
  return String(value, 2);
}
 
bool fall_detection(){
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
  
  /* Get the orientation of the sensor */
  uint8_t o = mma.getOrientation();
  switch (o) {
    case MMA8451_PL_PUF: 
      Serial.println("Portrait Up Front");
      break;
    case MMA8451_PL_PUB: 
      Serial.println("Portrait Up Back");
      break;    
    case MMA8451_PL_PDF: 
      Serial.println("Portrait Down Front");
      break;
    case MMA8451_PL_PDB: 
      Serial.println("Portrait Down Back");
      break;
    case MMA8451_PL_LRF: 
      Serial.println("Landscape Right Front");
      break;
    case MMA8451_PL_LRB: 
      Serial.println("Landscape Right Back");
      break;
    case MMA8451_PL_LLF: 
      Serial.println("Landscape Left Front");
      break;
    case MMA8451_PL_LLB: 
      Serial.println("Landscape Left Back");
      break;
    }
    delay(50);


    return false;
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


int sendBTData(String message){
  Serial.print(message);
  
  if(Serial.available()) 
  {
    mySerial.write(message);//Forward what Serial received to Software Serial Port
  }
  
  delay(10);
  return 0;
}




