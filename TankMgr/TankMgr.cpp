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

#include "PinControl.h"
#include "Ultrasonic.h"
#include "WireComm.h"
#include "Commands.h"
#include "Monitor.h"


PinControl		pinControl;
Ultrasonic		ultrasonic;
WireComm		wireComm;			// I2C functions and test commands
Commands		commands;
Monitor			monitor;

String			inputString;		// a String to hold incoming data
boolean			stringComplete;		// whether the string is complete


// Main setup routine
void setup() {

	Serial.begin(115200);
	while(!Serial) {}                   // Wait for it to be ready
    while ( Serial.available() ) {      // Clear buffer
        char inChar = (char)Serial.read();
    }

	inputString = "";
	inputString.reserve(200);	// reserve 200 bytes for the inputString
//	Serial.println( "" );

	// set the digital pin as output:
	pinControl = PinControl();
	pinControl.setupPins();

	pinControl.mainPowerOff();	// Set power off to motors and servos at startup

	ultrasonic = Ultrasonic();
	ultrasonic.setupForUltrasonic();

	// Wrapper for Wire class which writes to I2C
	wireComm = WireComm();
	wireComm.setupForWireComm( false );	// true for master, false for slave
	
	monitor = Monitor();

	commands = Commands();
	
	stringComplete = false;

	Serial.println( "Setup complete - ok" );
}


void parseCommand() {
	// Execute commands with parameters here
//	Serial.print( "In parseCommand with inputString length: " );
//	Serial.println( inputString.length() );
	if ( inputString.length() > 1 ) {	// Ensure more than just eol and maybe one character
		char firstChar = inputString[0];
		char param = inputString[1];
		switch ( firstChar ) {
				
			case 's': {			// s0\n or s90\n or s180\n
				int i = 1;
				int angle = 0;
				while ( ( param != '\n' ) && ( param != '\r' ) ) {
					angle = ( angle * 10 ) + ( param - '0' );
					i++;
					param = inputString[i];
				}
				Serial.print( "Set scanner to " );
				Serial.print( angle );
				Serial.println( " degrees" );
				pinControl.setAngle( angle );
				}
				break;
				
            case 'q': {			// q0\n or q90\n
				int i = 1;
				int angle = 0;
				while ( ( param != '\n' ) && ( param != '\r' ) ) {
					angle = ( angle * 10 ) + ( param - '0' );
					i++;
					param = inputString[i];
				}
				Serial.print( "Set scanner to " );
				Serial.print( angle );
				Serial.println( " degrees" );
				pinControl.setAngle( angle );
				delay( 200 );	// Wait for angle to be reached
				ultrasonic.ranger( angle );

				break;
            }
            default:
				break;
				
		}
	} else {
		Serial.println( "Error in parseCommand, unexpected inputString length" );
	}

}

// Main run loop
void loop() {
	
	//	Serial.print( "+" );
	if ( stringComplete ) {	// Check for and respond to serial commands
//		inputString = "New: " + inputString;
//		Serial.println( inputString );
		
		parseCommand();
		// Done processing local command, clear the string:
		inputString = "";
		stringComplete = false;
	}
	
	pinControl.toggle();			// Check if it is time to toggle LEDs
	
//	monitor.statusCheck();	// Take regular measurements and check important system states
}



// SerialEvent occurs whenever new data comes in the hardware serial RX. This
// routine is run between each time loop() runs, so using delay inside loop can
// delay response, as can delays here. Multiple bytes of data may be available.
void serialEvent() {
	while ( Serial.available() ) {			// DEBUG commands are here
		char inChar = (char)Serial.read();
		if ( inputString.length() == 0 ) {	// If first char of new string,
											// we may have a special 'quick' command:
											// one character, perform immediately
			switch ( inChar ) {
					
				case '0':
					pinControl.mainPowerOff();
					Serial.println( "Main power off" );
					return;
					
				case '1':
					pinControl.mainPowerOn();
					Serial.println( "Main power on" );
					return;
					
				case '2':
					pinControl.piPowerOff();
					Serial.println( "Pi power off" );
					return;
					
				case '3':
					pinControl.piPowerOn();
					Serial.println( "Pi power on" );
					return;
					
				case 'a':				// Scanner
					pinControl.setAngle( 0 );
					return;
				case 'b':				// Scanner
					pinControl.setAngle( 90 );
					return;
				case 'c':				// Scanner
					pinControl.setAngle( 180 );
					return;
					
				case 'l':
					pinControl.isRunning = !pinControl.isRunning;
					pinControl.interval = 1000;
					Serial.println( "Toggle LED, 1 second intervals" );
					return;
					
				case 'm':
					pinControl.isRunning = !pinControl.isRunning;
					pinControl.interval = 100;
					Serial.println( "Toggle LED, 1/10 second intervals" );
					return;
					
				case 'p':
					ultrasonic.pingSerialDisplay( 0, true );
					Serial.println( "Ping and print results" );
					return;
					
				case 'v':
					pinControl.powerToggle();
					Serial.println( "Main power toggle" );
					return;

				default:
					break;
			}
		}

		Serial.print( inChar );
		inputString += inChar;
		if ( ( inChar == '\n' ) || ( inChar == '\r' ) ) {
			stringComplete = true;
		}
	}
}
