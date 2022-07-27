# SBluetooth-Food-Tray

Building a Bluetooth-enabled food tray system to track individual portions and food waste for the Harvard Undergraduate Dining System

## Description

This Arduino-based system uses Bluetooth to track and log the presence of a tray at a particular serving station while also logging the portions of food taken. The tray tracking part is achieved by outfitting the serving station with a central Bluetooth device and outfitting the tray with a peripheral device. RSSI (Received Signal Strength Indicator) is used to measure the connection between the two and when it crosses a certain threshold (meaning the tray is close enough to a serving station), the timestamp is logged. The portion tracking is achieved by adding an IR distance sensor to the serving station so when a person reaches in to grab a portion, the IR beam is interrupted and the following timestamp is logged. The timestamps are matched up in post-processing to link the number of portions to each tray ID.

## Getting Started

### Dependencies

* Arduino desktop application
* There are several C++ libraries that must be installed. These are listed at the beginning of each file. Any library that does not exist locally can be downloaded through the Arduino desktop application in the top toolbar.

### Installing

* Install all files into a local directory in order to later upload to the respective Arduino boards.

### Executing program

* Since this is a Jupyter notebook, each block must be run sequentially using Shift + Enter. 

## Authors
* Bhushan Patel
* ENG-SCI 156: Signals and Communication class staff

## Acknowledgments
* Professor Flavio Calmon
* ENG-SCI 156: Signals and Communication class staff
