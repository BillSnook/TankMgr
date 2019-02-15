//
// Commands.cpp
// Library C++ code
// ----------------------------------
// Developed with embedXcode
// https://embedXcode.weebly.com
//
// Project 		TankMgr
//
// Created by 	William Snook, 1/13/19 11:25 AM
// 				billsnook
//
// Copyright 	(c) William Snook, 2019
// Licence		<#licence#>
//
// See 			Commands.h and ReadMe.txt for references
//


// Library header
#include "LEDs.h"
#include "Commands.h"
#include "WireComm.h"
#include "Ultrasonic.h"

extern Ultrasonic ultrasonic;
extern LEDs leds;

Commands::Commands() {
	
	pinMode( V_IN_PIN, INPUT );
	mode = initialMode;
	status = 0;
	stateBits = 0x4030;		// Test: @0
	last = 0;
	next = 0;
	range = 0;
	vIn = analogRead( V_IN_PIN );
	Serial.print("Commands initialization with vIn: ");
	Serial.println( vIn );
}

// REMOTE commands (writes from the Pi) are handled here
// Called if it seems to be a command and returns true if command succeeds
bool Commands::parseCommand( byte command, byte parameter ) {
	int succeeds = false;
	switch ( command ) {		// Modes determine how read data is structured
		case 'p':				// Ping
			mode = rangeMode;
			next = parameter;	// When next range is measured, this becomes an index
			ultrasonic.ranger( parameter );
			succeeds = true;
			break;
		case 's':				// Status
			mode = statusMode;
			succeeds = true;
			break;
		case 'v':				// Power relay enable
			if ( 0 == parameter ) {
				leds.mainPowerOff();
			} else {
				leds.mainPowerOn();
			}
			succeeds = true;
			break;
	}
	return succeeds;
}

// REMOTE reads are handled here
// Called when data is requested by a master when it wants to read data
bool Commands::handleRequest() {
	switch ( mode ) {
		case initialMode:
			Wire.write( (uint8_t)0xAA );
			Wire.write( (uint8_t)0x55 );
			break;

		case statusMode:
			vIn = analogRead( V_IN_PIN );
			Wire.write( (uint8_t)((vIn >> 8) & 0xFF) );
			Wire.write( (uint8_t)(vIn & 0xFF) );
			Wire.write( (uint8_t)((stateBits >> 8) & 0xFF) );
			Wire.write( (uint8_t)(stateBits & 0xFF) );
			mode = statusMode;
			break;
			
		case rangeMode:
			last = ultrasonic.last;
			range = ultrasonic.range;
			Wire.write( (uint8_t)((last >> 8) & 0xFF) );
			Wire.write( (uint8_t)(last & 0xFF) );
			Wire.write( (uint8_t)((range >> 8) & 0xFF) );
			Wire.write( (uint8_t)(range & 0xFF) );
			break;
			
		default:
			return false;
			break;
	}
	return true;
}
