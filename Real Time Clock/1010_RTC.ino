#include <SPI.h>
#include <WiFiNINA.h>


///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = "Harvard University";        // your network SSID (name)

int status = WL_IDLE_STATUS;     // the Wifi radio's status

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < "1.0.0") {
    Serial.println("Please upgrade the firmware");
  }

  WiFi.config(IPAddress(10, 130, 22, 67), IPAddress(8, 8, 8, 8), IPAddress(10, 130, 22, 1), IPAddress(255, 255, 255, 0));

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid);

    // wait 10 seconds for connection:
    delay(10000);
  }

  WiFi.setDNS(IPAddress(8, 8, 8, 8));

  // you're connected now, so print out the data:
  Serial.print("You're connected to the network");
}

void loop() {
  Serial.println(WiFi.getTime());
  delay(10000);
}
