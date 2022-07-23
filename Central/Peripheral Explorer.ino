/*
  Peripheral Explorer

  This example scans for BLE peripherals until one with a particular name ("LED")
  is found. Then connects, and discovers + prints all the peripheral's attributes.

  The circuit:
  - Arduino MKR WiFi 1010, Arduino Uno WiFi Rev2 board, Arduino Nano 33 IoT,
    Arduino Nano 33 BLE, or Arduino Nano 33 BLE Sense board.

  You can use it with another board that is compatible with this library and the
  Peripherals -> LED example.

  This example code is in the public domain.
*/

#include <ArduinoBLE.h>
//#include <SharpIR.h>
//#include <StandardCplusplus.h>
#include <vector>
#include <numeric>
using std::vector; 
#include "RTClib.h"
#include <stdio.h>
#include <stdlib.h>
#include <WiFiNINA.h>
#include <SPI.h>

char ssid[] = "Harvard University";        // your network SSID (name)
int status = WL_IDLE_STATUS;     // the Wifi radio's status

char tray1[] = "6e400001-b5a3-f393-e0a9-e50e24dcca9e";

RTC_DS3231 rtc;

int count = 0;
int plate_count = 1;
int grssi;
void setup() {
  Serial.begin(9600);
  while (!Serial);

//  WiFi.config(IPAddress(10, 130, 22, 67), IPAddress(8, 8, 8, 8), IPAddress(10, 130, 22, 1), IPAddress(255, 255, 255, 0));
//  
//  // attempt to connect to Wifi network:
//  while (status != WL_CONNECTED) {
//    Serial.print("Attempting to connect to network: ");
//    Serial.println(ssid);
//    // Connect to WPA/WPA2 network:
//    status = WiFi.begin(ssid);
//
//    // wait 10 seconds for connection:
//    delay(10000);
//  }
//
//  WiFi.setDNS(IPAddress(8, 8, 8, 8));
//
//  // you're connected now, so print out the data:
//  Serial.println("You're connected to the network");
//  
//  Serial.println("----------------------------------------");
//  printData1();
//  Serial.println("----------------------------------------");

  // begin initialization
  
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");

    while (1);
  }

  #ifndef ESP8266
  while (!Serial); // wait for serial port to connect. Needed for native USB
#endif

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
//    delay(2000);
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


  Serial.println("BLE Central - Peripheral Explorer");

  // start scanning for peripherals
  BLE.scan();
}

void loop() {
  // check if a peripheral has been discovered
  BLEDevice peripheral = BLE.available();
  DateTime now = rtc.now();

  if (peripheral) {
    // get time of discovery 
//    long int now = WiFi.getTime();
   
    // see if peripheral is a tray
    if (peripheral.localName() == "Adafruit Bluefruit LE") {
      getrssi(peripheral);
      if (grssi > -65) {
        // discovered a peripheral, print out address, local name, advertised service, and time discovered
        Serial.print("Found ");
        Serial.print(peripheral.address());
        Serial.print(" '");
        Serial.print(peripheral.localName());
        Serial.print("' ");
        Serial.print(peripheral.advertisedServiceUuid());
        Serial.println();
        // stop scanning
        BLE.stopScan();
        explorerPeripheral(peripheral);
      }

//      Serial.println(WiFi.getTime());
      

      // peripheral disconnected, we are done
//      while (1) {
//        // do nothing
//      }
    }
  }

}

void explorerPeripheral(BLEDevice peripheral) {
  // connect to the peripheral
  Serial.println("Connecting ...");

  if (peripheral.connect()) {
    Serial.println("Connected");
  } else {
    Serial.println("Failed to connect!");
    return;
  }

  // Retrieve Advertising UUID
  Serial.println("Retrieving UUID ...");
  if (peripheral.advertisedServiceUuid()) {
    Serial.print("UUID discovered: ");
    Serial.println( peripheral.advertisedServiceUuid());
  } else {
    Serial.println("Attribute discovery failed!");
    peripheral.disconnect();
    return;
  }

  // Check if UUID matches with one of the trays
  Serial.println("Determining Tray Number...");
  if (peripheral.advertisedServiceUuid() == tray1) {
    Serial.print("UUID Matched to Tray #: ");
    Serial.println(plate_count);
    Serial.print("Time Matched: ");
    Serial.println(rtc.now().unixtime());
  }
 

  while (1) {
  getrssi(peripheral);
//  Serial.print("Global RSSI: ");
//  Serial.println(grssi);
  if (grssi < -65) {
    Serial.print("RSSI: ");
    Serial.println(grssi);
    Serial.println("Disconnecting...");
    peripheral.disconnect();
    Serial.println("Adafruit Disconnected");
    plate_count++;
    BLE.scan();
    break;
  }
  }
    

  

  // read and print device name of peripheral
//  Serial.println();
//  Serial.print("Device name: ");
//  Serial.println(peripheral.deviceName());
//  Serial.print("Appearance: 0x");
//  Serial.println(peripheral.appearance(), HEX);
//  Serial.println();

  // loop the services of the peripheral and explore each
//  for (int i = 0; i < peripheral.serviceCount(); i++) {
//    BLEService service = peripheral.service(i);
//
//    exploreService(service);
//  }

//  Serial.println();

  // we are done exploring, disconnect
//  Serial.println("Disconnecting ...");
//  peripheral.disconnect();
//  Serial.println("Disconnected");
}

void exploreService(BLEService service) {
  // print the UUID of the service
  Serial.print("Service ");
  Serial.println(service.uuid());

  // loop the characteristics of the service and explore each
  for (int i = 0; i < service.characteristicCount(); i++) {
    BLECharacteristic characteristic = service.characteristic(i);

    exploreCharacteristic(characteristic);
  }
}

void exploreCharacteristic(BLECharacteristic characteristic) {
  // print the UUID and properties of the characteristic
  Serial.print("\tCharacteristic ");
  Serial.print(characteristic.uuid());
  Serial.print(", properties 0x");
  Serial.print(characteristic.properties(), HEX);

  // check if the characteristic is readable
  if (characteristic.canRead()) {
    // read the characteristic value
    characteristic.read();

    if (characteristic.valueLength() > 0) {
      // print out the value of the characteristic
      Serial.print(", value 0x");
      printData(characteristic.value(), characteristic.valueLength());
    }
  }
  Serial.println();

  // loop the descriptors of the characteristic and explore each
  for (int i = 0; i < characteristic.descriptorCount(); i++) {
    BLEDescriptor descriptor = characteristic.descriptor(i);

    exploreDescriptor(descriptor);
  }
}

void exploreDescriptor(BLEDescriptor descriptor) {
  // print the UUID of the descriptor
  Serial.print("\t\tDescriptor ");
  Serial.print(descriptor.uuid());

  // read the descriptor value
  descriptor.read();

  // print out the value of the descriptor
  Serial.print(", value 0x");
  printData(descriptor.value(), descriptor.valueLength());

  Serial.println();
}

void printData(const unsigned char data[], int length) {
  for (int i = 0; i < length; i++) {
    unsigned char b = data[i];

    if (b < 16) {
      Serial.print("0");
    }

    Serial.print(b, HEX);
  }
}


void getrssi(BLEDevice peripheral) {
  count = 0;
  vector<int> rssivals;
  while(count < 1000) {
    rssivals.push_back(peripheral.rssi());
    count++;
  }
  int sum = std::accumulate(rssivals.begin(),rssivals.end(),0);
  grssi = sum / (int) rssivals.size();
}

//void printData1() {
//  Serial.println("Board Information:");
//  // print your board's IP address:
//  IPAddress ip = WiFi.localIP();
//  Serial.print("IP Address: ");
//  Serial.println(ip);
//
//  Serial.println();
//  Serial.println("Network Information:");
//  Serial.print("SSID: ");
//  Serial.println(WiFi.SSID());
//
//  // print the received signal strength:
//  long rssi = WiFi.RSSI();
//  Serial.print("signal strength (RSSI):");
//  Serial.println(rssi);
//
//  byte encryption = WiFi.encryptionType();
//  Serial.print("Encryption Type:");
//  Serial.println(encryption, HEX);
//  Serial.println();
//}
