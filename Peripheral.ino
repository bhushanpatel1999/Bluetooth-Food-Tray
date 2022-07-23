#include <Adafruit_ATParser.h>
#include <Adafruit_BLE.h>
#include <Adafruit_BLEBattery.h>
#include <Adafruit_BLEEddystone.h>
#include <Adafruit_BLEGatt.h>
#include <Adafruit_BLEMIDI.h>
#include <Adafruit_BluefruitLE_SPI.h>
#include <Adafruit_BluefruitLE_UART.h>


/*********************************************************************
 This is an example for our nRF51822 based Bluefruit LE modules

 Pick one up today in the adafruit shop!

 Adafruit invests time and resources providing this open source code,
 please support Adafruit and open-source hardware by purchasing
 products from Adafruit!

 MIT license, check LICENSE for more information
 All text above, and the splash screen below must be included in
 any redistribution
*********************************************************************/

#include <Arduino.h>
//#include <bluefruit.h>
//#include <ble_gap.h>
#include <SPI.h>
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"
#if SOFTWARE_SERIAL_AVAILABLE
  #include <SoftwareSerial.h>
#endif

#include "Config.h"

/*=========================================================================
    APPLICATION SETTINGS

    FACTORYRESET_ENABLE       Perform a factory reset when running this sketch

                              Enabling this will put your Bluefruit LE module
                              in a 'known good' state and clear any config
                              data set in previous sketches or projects, so
                              running this at least once is a good idea.

                              When deploying your project, however, you will
                              want to disable factory reset by setting this
                              value to 0.  If you are making changes to your
                              Bluefruit LE device via AT commands, and those
                              changes aren't persisting across resets, this
                              is the reason why.  Factory reset will erase
                              the non-volatile memory where config data is
                              stored, setting it back to factory default
                              values.
    -----------------------------------------------------------------------*/
    #define FACTORYRESET_ENABLE         1
    #define URL                         "http://www.adafruit.com"
/*=========================================================================*/


// Create the bluefruit object, either software serial...uncomment these lines

SoftwareSerial bluefruitSS = SoftwareSerial(BLUEFRUIT_SWUART_TXD_PIN, BLUEFRUIT_SWUART_RXD_PIN);

Adafruit_BluefruitLE_UART ble(bluefruitSS, BLUEFRUIT_UART_MODE_PIN,
                      BLUEFRUIT_UART_CTS_PIN, BLUEFRUIT_UART_RTS_PIN);


/* ...or hardware serial, which does not need the RTS/CTS pins. Uncomment this line */
//Adafruit_BluefruitLE_UART ble(BLUEFRUIT_HWSERIAL_NAME, BLUEFRUIT_UART_MODE_PIN);

/* ...hardware SPI, using SCK/MOSI/MISO hardware SPI pins and then user selected CS/IRQ/RST */
//Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);

/* ...software SPI, using SCK/MOSI/MISO user-defined SPI pins and then user selected CS/IRQ/RST */
//Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_SCK, BLUEFRUIT_SPI_MISO,
//                             BLUEFRUIT_SPI_MOSI, BLUEFRUIT_SPI_CS,
//                             BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);

// A small helper
void error(const __FlashStringHelper*err) {
  Serial.println(err);
  while (1);
}

/**************************************************************************/
/*!
    @brief  Sets up the HW an the BLE module (this function is called
            automatically on startup)
*/
/**************************************************************************/

// UUID : 25df76df-a120-4171-aeff-f6b0a3bcb84f
//const uint8_t CUSTOM_UUID[] =
//{
//    0x4F, 0xB8, 0xBC, 0xA3, 0xB0, 0xF6, 0xFF, 0xAE,
//    0x71, 0x41, 0x20, 0xA1, 0xDF, 0x76, 0xDF, 0x25
//};
//
//BLEUuid uuid = BLEUuid(CUSTOM_UUID);


void setup(void)
{
  while (!Serial);  // required for Flora & Micro
  delay(500);

  Serial.begin(115200);
  Serial.println(F("Adafruit Bluefruit UriBeacon Example"));
  Serial.println(F("------------------------------------"));

  /* Initialise the module */
  Serial.print(F("Initialising the Bluefruit LE module: "));

  if ( !ble.begin(VERBOSE_MODE) )
  {
    error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
  }
  Serial.println( F("OK!") );

  if ( FACTORYRESET_ENABLE )
  {
    /* Perform a factory reset to make sure everything is in a known state */
    Serial.println(F("Performing a factory reset: "));
    if ( ! ble.factoryReset() ){
      error(F("Couldn't factory reset"));
    }
  }

  /* Disable command echo from Bluefruit */
  ble.echo(false);

  Serial.println("Requesting Bluefruit info:");
  /* Print Bluefruit information */
  ble.info();

  // Prompt user to migrate to EddyStone
  Serial.println(F("Please consider to migrate to EddyStone since Google has retired uribeacon"));

  /* Set EddyStone URL beacon data */
  Serial.println(F("Setting uri beacon to Adafruit website: "));

  // Older firmware use AT+BLEURIBEACON command
  if (! ble.sendCommandCheckOK(F( "AT+BLEURIBEACON=" URL ))) {
    error(F("Couldnt set, is URL too long !?"));
  }

  Serial.println(F("**************************************************"));
  Serial.println(F("Please use Google Physical Web application to test"));
  Serial.println(F("**************************************************"));

  // Set up and start advertising
//  startAdv();
//
//  Serial.println("Advertising started"); 
}

//void startAdv(void)
//{   
//  // Note: The entire advertising packet is limited to 31 bytes!
//  
//  // Advertising packet
//  // Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
//  Bluefruit.Advertising.addTxPower();
//
//  // Preferred Solution: Add a custom UUID to the advertising payload, which
//  // we will look for on the Central side via Bluefruit.Scanner.filterUuid(uuid);
//  // A valid 128-bit UUID can be generated online with almost no chance of conflict
//  // with another device or etup
//  Bluefruit.Advertising.addUuid(uuid);
//
//  /*
//  // Alternative Solution: Manufacturer Specific Data (MSD)
//  // You could also send a custom MSD payload and filter for the 'Company ID'
//  // via 'Bluefruit.Scanner.filterMSD(CID);', although this does require a
//  // valid CID, which is why the UUID method above is more appropriate in
//  // most situations. For a complete list of valid company IDs see:
//  // https://www.bluetooth.com/specifications/assigned-numbers/company-identifiers
//  // For test purposes, 0xFFFF CAN be used, but according to the Bluetooth SIG:
//  // > "This value may be used in the internal and interoperability tests before a
//  // >  Company ID has been assigned. This value shall not be used in shipping end
//  // >  products."
//  uint8_t msd_payload[4]; // Two bytes are required for the CID, so we have 2 bytes user data, expand as needed
//  uint16_t msd_cid = 0xFFFF;
//  memset(msd_payload, 0, sizeof(msd_payload));
//  memcpy(msd_payload, (uint8_t*)&msd_cid, sizeof(msd_cid));
//  msd_payload[2] = 0x11;
//  msd_payload[3] = 0x22;
//  Bluefruit.Advertising.addData(BLE_GAP_AD_TYPE_MANUFACTURER_SPECIFIC_DATA, msd_payload, sizeof(msd_payload));
//  */
//
//  // Not enough room in the advertising packet for name
//  // so store it in the Scan Response instead
//  Bluefruit.ScanResponse.addName();
//
//  /* Start Advertising
//   * - Enable auto advertising if disconnected
//   * - Interval:  fast mode = 20 ms, slow mode = 152.5 ms
//   * - Timeout for fast mode is 30 seconds
//   * - Start(timeout) with timeout = 0 will advertise forever (until connected)
//   * 
//   * For recommended advertising interval
//   * https://developer.apple.com/library/content/qa/qa1931/_index.html
//   */
//  Bluefruit.Advertising.restartOnDisconnect(true);
//  Bluefruit.Advertising.setInterval(32, 244);    // in units of 0.625 ms
//  Bluefruit.Advertising.setFastTimeout(30);      // number of seconds in fast mode
//  Bluefruit.Advertising.start();
//}
/**************************************************************************/
/*!
    @brief  Constantly poll for new command or response data
*/
/**************************************************************************/
void loop(void)
{

}

/**************************************************************************/
/*!
    @brief  Checks for user input (via the Serial Monitor)
*/
/**************************************************************************/
void getUserInput(char buffer[], uint8_t maxSize)
{
  memset(buffer, 0, maxSize);
  while( Serial.available() == 0 ) {
    delay(1);
  }

  uint8_t count=0;

  do
  {
    count += Serial.readBytes(buffer+count, maxSize);
    delay(2);
  } while( (count < maxSize) && !(Serial.available() == 0) );
}
