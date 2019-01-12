//
// TankMgr
//
// Manage power for tank project
// Developed with [embedXcode](http://embedXcode.weebly.com)
//
// Author 		William Snook
// 				billsnook
//
// Date			1/4/19 9:13 AM
// Version		0.1
//
// Copyright	© William Snook, 2019
// Licence		None
//
// See         ReadMe.txt for references
//

#include <Arduino.h>

#include "LEDs.h"
#include "Ultrasonic.h"
#include "WireComm.h"


LEDs			leds;
Ultrasonic		ultrasonic;
WireComm		wireComm;			// I2C functions and test commands

String			inputString;		// a String to hold incoming data
boolean			stringComplete;		// whether the string is complete

boolean			mainPowerState;

void setup() {

	Serial.begin(115200);
	while(!Serial) {}	// Wait for it to be ready

	inputString = "";
	inputString.reserve(200);	// reserve 200 bytes for the inputString
	Serial.println( "" );

	// set the digital pin as output:
	leds = LEDs();
	leds.setupForLEDs();

	mainPowerState = false;
	leds.mainPowerOff();

	ultrasonic = Ultrasonic();
	ultrasonic.setupForUltrasonic();

	wireComm = WireComm();
	wireComm.setupForWireComm( false );	// true for master, false for slave

	stringComplete = false;

	Serial.println( "Setup complete - ok" );
}

void loop() {
	// here is where you'd put code that needs to be running all the time.

	//	Serial.print( "+" );
	if ( stringComplete ) {
		inputString = "New: " + inputString;
		Serial.println( inputString );
		// clear the string:
		inputString = "";
		stringComplete = false;
	}

	leds.toggle();

	wireComm.runWireComm();
}

// SerialEvent occurs whenever a new data comes in the hardware serial RX. This
// routine is run between each time loop() runs, so using delay inside loop can
// delay response. Multiple bytes of data may be available.
void serialEvent() {
	while ( Serial.available() ) {
		char inChar = (char)Serial.read();
		if ( inputString.length() == 0 ) {	// If first char of new string
			Serial.print( inChar );
			switch ( inChar ) {

				case 'l':
					leds.isRunning = !leds.isRunning;
					leds.interval = 1000;
					continue;

				case 'm':
					leds.isRunning = !leds.isRunning;
					leds.interval = 100;
					continue;

				case 'p':
					if ( mainPowerState ) {
						leds.mainPowerOff();
					} else {
						leds.mainPowerOn();
					}
					mainPowerState = !mainPowerState;
					continue;

				case 'r':								// buffer size is 32
					wireComm.readWireComm( 20 );		// We never get more than requested, we can get less
					continue;

				case 'w':
					wireComm.writeWireComm( "Data" );
					continue;

				default:
					break;

			}
		}
		inputString += inChar;
		if ( ( inChar == '\n' ) || ( inChar == '\r' ) ) {
			stringComplete = true;
		}
	}
}
