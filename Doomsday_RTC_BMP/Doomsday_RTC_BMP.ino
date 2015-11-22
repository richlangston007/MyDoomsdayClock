// Doomsday clock example - Ticking HH:MM:SS
// Copyright (c) 2012 Devon Sean McCullough
//
// This example is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either version 2.1
// of the License, or (at your option) any later version.
// 
// See file LICENSE.txt for further informations on licensing terms.

#include <Doomsday.h>
#include <Time.h>  
#include <Wire.h>  
#include <DS1307RTC.h>  // a basic DS1307 library that returns time as a time_t
//temperature sensor
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>

Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);
Doomsday doom;
static unsigned int dots = (256 * B00001000) + B00000000;
int i=0;
int j=0;
float temperature;
int firstTime=1;

void setup ()
{
 
  doom.begin (72);	// 72 ms ~~ 14 Hz
  Serial.begin(9600);
  while (!Serial) ; // wait until Arduino Serial Monitor opens
  setSyncProvider(RTC.get);   // the function to get the time from the RTC
  if(timeStatus()!= timeSet) 
     Serial.println("Unable to sync with the RTC");
  else
     Serial.println("RTC has set the system time");  
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(month());
  Serial.print(" ");
  Serial.print(year()); 
  Serial.println();     
  doom.setClock (second() , minute(), hour(), day(), month(), year());
  //
  //  Set up temp/altitude sensor
  Serial.println("Pressure Sensor Test"); Serial.println("");
  
  /* Initialise the sensor */

  if(!bmp.begin())
  {
    /* There was a problem detecting the BMP085 ... check your connections */
   Serial.print("Ooops, no BMP085 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }

  
  /* Display some basic information on this sensor */
  displaySensorDetails();
  
}

void loop ()
{
  //Read the brightness from the 
  int sensorValue = analogRead(3);
//  Serial.println(sensorValue);

  //Get temperature from BMP
  if (( second()==0 ) & (firstTime)) {
    firstTime=0;
    bmp.getTemperature(&temperature);
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" C");
  }
  if (second()==1) firstTime=1;
 float tempF=temperature*9/5 + 32;   
 


  //Not sure if this is needed, but let's sync the doom.Clock to the RTC once a day
  if (( hour() == 3) & ( second() == 0) & (minute()==0)) doom.setClock (second() , minute(), hour(), day(), month(), year()); 
  if ( i<12) { 
        dots = (dots >> 1);
        i++;
      }
    else {
       i=0;
       dots = (256 * B00001000) + B00000000;
    }
  if (sensorValue<95) dots=0;
  doom.secondsTick ();
  //doom.printd ("%H:%M:%S", dots);
  if ( (second()<24) || ((second()>29) & (second()<54))) {
    
    doom.printd ("%H%(:%)%M%(:%)%S", dots);
  } else {
 //   doom.setDecimal(tempF); // Next we feed the numbers in to the "setDecimal" function. It is stored as "%i".
 //   doom.printd ("%i"); // In the print function "doom.printd" the number from "setDecimal" value is represented as "%i".
      doom.setdeciKelvins((temperature+273.15)*10s);
      doom.printd ("%F\33F");
  }
  //doom.printd (":H:E:%[L%]LO%(O%)", dots);
 
}
void printDigits(int digits){
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}


//**************************************************************************/
/*
    Displays some basic information on this sensor from the unified
    sensor API sensor_t type (see Adafruit_Sensor for more information)
*/
/**************************************************************************/
void displaySensorDetails(void)
{
  sensor_t sensor;
//  bmp.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" hPa");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" hPa");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" hPa");  
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}



