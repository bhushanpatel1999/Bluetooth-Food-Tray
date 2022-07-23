/*SHARP GP2Y0A21YK0F IR distance sensor with Arduino and SharpIR library example code. More info: https://www.makerguides.com */

// Include the library:
#include <SharpIR.h>
#include <StandardCplusplus.h>
//#include <Vector.h>
#include <vector>
// using std::vector;
using std::vector; 

// Date and time functions using a DS3231 RTC connected via I2C and Wire lib
#include "RTClib.h"

RTC_DS3231 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// Define model and input pin:
#define IRPin A0
#define model 1080

// Create variable to store the distance:
int distance_cm;

// Create vector to store timestamps of disruptions
// timestamps will store the unixtime of each stamp
// 
//const int ELEMENT_COUNT_MAX = 1;
//int storage_array[ELEMENT_COUNT_MAX];
//Vector<int> timestamps(storage_array);

vector<long int> timestamps;
long int temptime = 0;

// Create count variable to count the # of disruptions
long int count = 0;


/* Model :
  GP2Y0A02YK0F --> 20150
  GP2Y0A21YK0F --> 1080
  GP2Y0A710K0F --> 100500
  GP2YA41SK0F --> 430
*/

// Create a new instance of the SharpIR class:
SharpIR mySensor = SharpIR(IRPin, model);

void setup() {
  // Begin serial communication at a baudrate of 9600:
  Serial.begin(57600);

  long int initial = 0;
  timestamps.push_back(initial);

  #ifndef ESP8266
  while (!Serial); // wait for serial port to connect. Needed for native USB
#endif

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    abort();
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

  // When time needs to be re-set on a previously configured device, the
  // following line sets the RTC to the date & time this sketch was compiled
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  // This line sets the RTC with an explicit date & time, for example to set
  // January 21, 2014 at 3am you would call:
  // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
}

void loop() {
  // Get a distance measurement and store it as distance_cm:
  distance_cm = mySensor.distance();
  
  // Get time from RTC and store is as now:
  DateTime now = rtc.now();

  // Get length of timestamps and starts_ends vectors
  // int time_size = sizeof(timestamps)/sizeof(int);
  // count variable to know last index of timestamps vector
  
  
  // if something disrupts the usual distance reading (i.e someone is taking a scoop)
  if (distance_cm < 39) {
    // Print the measured distance to the serial monitor:
//    Serial.print("Last Timestamp in Vector: ");
//    Serial.println(timestamps[count]);
//    Serial.print("Whole vector: ");
//    Serial.println(timestamps.size());
    // determine if this is the start of a scoop
    if (now.unixtime() > (temptime + 1)) {
      // add current unixtime to timestamps vector
      Serial.print("New Scoop Detected at Unixtime: ");
      Serial.println(now.unixtime());
      Serial.print("Mean distance: ");
      Serial.print(distance_cm);
      Serial.println(" cm");
      timestamps.push_back(now.unixtime());
      count++;
      Serial.print("Timestamp Added: ");
      Serial.println(timestamps[count]);
      Serial.print("Total Scoops Detected: ");
      Serial.println(count);
      Serial.println();
      }
     temptime = now.unixtime();
  }

  

  /*Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" (");
  Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
  Serial.print(") ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();

  Serial.print(" since midnight 1/1/1970 = ");
  Serial.print(now.unixtime());
  Serial.print("s = ");
  Serial.print(now.unixtime() / 86400L);
  Serial.println("d"); */
 
  delay(100);
}
