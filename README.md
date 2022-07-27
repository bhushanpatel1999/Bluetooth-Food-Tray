# Bluetooth-Food-Tray

Building a Bluetooth-enabled food tray system to track individual portions and food waste for the Harvard Undergraduate Dining System

## Description

This Arduino-based system uses Bluetooth to track and log the presence of a tray at a particular serving station while also logging the portions of food taken. The tray tracking part is achieved by outfitting the serving station with a central Bluetooth device and outfitting the tray with a peripheral device. RSSI (Received Signal Strength Indicator) is used to measure the connection between the two and when it crosses a certain threshold (meaning the tray is close enough to a serving station), the timestamp is logged. The portion tracking is achieved by adding an IR distance sensor to the serving station so when a person reaches in to grab a portion, the IR beam is interrupted and the following timestamp is logged. The timestamps are matched up in post-processing to link the number of portions to each tray ID.

## Getting Started

### Dependencies

* Arduino desktop application
* Arduino Uno microcontroller
* Arduino MKR WiFi 1010 microcontroller
* Adafruit DS3231 Precision RTC module
* Adafruit SHARP IR distance sensor
* Adafruit Bluefruit LE UART device
* There are several C++ libraries that must be installed. These are listed at the beginning of each file. Any library that does not exist locally can be downloaded through the Arduino desktop application in the top toolbar.

### Installing

* Install all files into a local directory in order to later upload to the respective Arduino boards.

### Executing program

* Set up the serving station devices by connecting the RTC module and the SHARP IR sensor to the MKR WiFi microcontroller (refer to https://docs.arduino.cc/built-in-examples/ for help with specific wiring diagrams).
* Set up the tray devices by connecting the Bluefruit device to the Uno microcontroller (refer to https://docs.arduino.cc/built-in-examples/ for help with specific wiring diagrams).
* Connect MKR Wifi board to a computer and upload files from the Arduino application in the following order:
  * Upload RTC file first to configure the clock through the WiFi
  * Upload the files within the "Central" folder to configure the Bluetooth specifics of the central device
  * Upload the IR sensor file to activate the IR beam
* Connect the Uno board to a computer and upload files from the Arduino application in the following order:
  * Upload RTC file first to configure the clock through the WiFi
  * Upload the files within the "Peripheral" folder to configure the Bluetooth specifics of the peripheral device

## Authors
* Bhushan Patel
* ENG-SCI 156: Signals and Communication class staff

## Acknowledgments
* Professor Flavio Calmon
* ENG-SCI 156: Signals and Communication class staff
