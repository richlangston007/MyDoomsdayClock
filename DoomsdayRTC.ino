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


Doomsday doom;
static unsigned int dots = (256 * B00001000) + B00000000;
int i=0;
int j=0;

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
  
}

void loop ()
{
  int sensorValue = analogRead(3);
  Serial.println(sensorValue);

  //Periodically print the time to the serial so we can see if there's drift
  if ( second()==0 ) {
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
  }
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
  doom.printd ("%H%(:%)%M%(:%)%S", dots);
  //doom.printd (":H:E:%[L%]LO%(O%)", dots);
 
}
void printDigits(int digits){
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}
