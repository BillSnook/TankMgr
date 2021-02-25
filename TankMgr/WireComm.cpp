//
// WireComm.cpp
// Library C++ code
//
// Created by 	William Snook, 7/22/18 5:02 PM
// 				billsnook
//
// Copyright 	(c) William Snook, 2018
// Licence		<#licence#>
//
// See 			WireComm.h and ReadMe.txt for references
//


// Library header
#include "Arduino.h"
#include "TankMgr.h"
#include "WireComm.h"
#include "Commands.h"


extern	Commands		commands;


WireComm::WireComm() {
	
	isSetup = false;		// to be sure
}

void WireComm::setupForWireComm( bool beMaster ) {
	
	if ( isSetup ) {
#ifdef SERIAL_COMMANDS
		Serial.println( "WireComm is already setup." );
#endif  // SERIAL_COMMANDS
		return;
	}
	isSetup = true;							    // only done once

//#ifdef SERIAL_COMMANDS
//    Serial.print("I2C slave address set: ");        // Debug
//    Serial.println( I2C_SLAVE_ADDRESS );
//#endif  // SERIAL_COMMANDS

	Wire.begin( I2C_SLAVE_ADDRESS );			// join i2c bus as slave with address #8
	Wire.onReceive(receiveEvent);				// register receive event for writes from master
	Wire.onRequest(requestEvent);				// register request event for read to master
}

void WireComm::resetForWireComm() {
	
	Wire.end();										// Quit i2c bus
}

// MARK: These I2C slave callback routines handle writes and reads from the I2C master

// REMOTE I2C data requests from master are handled here
// function that executes whenever data is requested by an I2C master when it reads
// this function is registered as an event, see setup()
void WireComm::requestEvent() {
	
//	// Check mode then respond with appropriate data
	commands.handleI2CRequest();
}

// REMOTE I2C data commands from master are handled here
// function that executes whenever data is received from I2C master when it writes
// this function is registered as an event, see setup()
void WireComm::receiveEvent( int howMany ) {
//#ifdef SERIAL_COMMANDS
//	Serial.print("Got receiveEvent to accept write, howMany: ");
//	Serial.println(howMany);
//#endif  // SERIAL_COMMANDS

	if ( howMany == 2 ) {			// Potential command
		if ( Wire.available() == 2 ) {
			char command = Wire.read();
			byte parameter = Wire.read();
#ifdef SERIAL_COMMANDS
            Serial.print( "command: " );
            Serial.print( command );
            Serial.print( ", parameter: " );
            Serial.println( parameter );
//            Serial.println( "" );                // print the end of line
#endif  // SERIAL_COMMANDS
			commands.parseI2CCommand( command, parameter );	// Handled here
			return;
		}
	}
    // Unexpected command format
#ifdef SERIAL_COMMANDS
    Serial.print( howMany );        // print the end of line
    Serial.print( ": < " );         // print the end of line
	while (0 < Wire.available()) {	// loop through all
		char c = Wire.read();		// receive bytes as a character
		Serial.print(c);			// print the character
	}
	Serial.println( " >" );		    // print the end of line
#endif  // SERIAL_COMMANDS
}
