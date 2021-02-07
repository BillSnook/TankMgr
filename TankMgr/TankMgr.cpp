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

#include "TankMgr.h"
#include "PinControl.h"
#include "WireComm.h"
#include "Commands.h"
#include "Monitor.h"


PinControl		pinControl;
Commands		commands;
WireComm        wireComm;           // I2C functions and test commands
Monitor			monitor;

String			inputString;		// a String to hold incoming data
boolean			stringComplete;		// whether the string is ready to be read


// Main setup routine
void setup() {

	Serial.begin(115200);
    
    stringComplete = false;
	while(!Serial) {}                   // Wait for it to be ready
    while ( Serial.available() ) {      // Clear buffer
        Serial.read();
    }

	inputString = "";
	inputString.reserve(200);	// reserve 200 bytes for the inputString

	// Setup the digital pins as input or output and set defaults as needed
	pinControl.setupPins();

	// Wrapper for Wire class which communicates on I2C to Pi
	wireComm.setupForWireComm( false );	// true for master, false for slave
	
	Serial.println( "Setup complete 5" );
}


// Main run loop
void loop() {
    
    //    Serial.print( "+" );
    if ( stringComplete ) {    // Check for and respond to serial commands
//        inputString = "New: " + inputString;
//        Serial.println( inputString );
        
        parseDbgCommand();  // Parse inputString and execute commands
        // Done processing local command, clear the string:
        inputString = "";
        stringComplete = false;
    }
    
    pinControl.toggle();            // Check if it is time to toggle LEDs
    
//    monitor.statusCheck();    // Take regular measurements and check important system states
}



void parseDbgCommand() {       // Execute test commands from serial port
//    Serial.print( "In parseDbgCommand with inputString < " );
//    Serial.print( inputString );
//    Serial.print( " >, length: " );
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
//				pinControl.setAngle( angle );
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
//				pinControl.setAngle( angle );
				delay( 200 );	// Wait for angle to be reached

				break;
            }
            default:
				break;
		}
        
	} else {    // One character commands

        char firstChar = inputString[0];
        switch ( firstChar ) {
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
                
            default:
                Serial.println( "Error in parseDbgCommand, unexpected inputString entry or length" );
                break;
        }

	}

}

// NOTE: - Serial monitor in the Arduino IDE sends a newline with each Send button click.
// We need to manage that here. We are currently trying to use a single letter as
// a 'quick' command but we need to fix that.

// SerialEvent occurs whenever new data comes in the hardware serial RX. This
// routine is run between each time loop() runs, so using delay inside loop can
// delay response, as can delays here. Multiple bytes of data may be available.
// This is the USB power/serial connection from a terminal on a development computer.
// Not used when running autonomously; i2c from companion will send controls.
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
					return;
				case 'b':				// Scanner
					return;
				case 'c':				// Scanner
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
