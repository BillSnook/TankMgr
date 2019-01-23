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
#include "Commands.h"
#include "WireComm.h"


Commands::Commands() {
	
	mode = initialMode;
	status = 0;
	pinMode( V_IN_PIN, INPUT );
//	delay( 20 );		// mSec
	vIn = analogRead( V_IN_PIN );
	Serial.print("Commands initialization with vIn: ");
	Serial.println( vIn );
	stateBits = 0x4030;		// Test: @0
}

// Called if it seems to be a command and returns true if command succeeds
bool Commands::parseCommand( unsigned char command ) {
	Serial.print( "In parseCommand: " );
	Serial.println( command );

	bool succeeds = false;
	switch ( command ) {
		case 'r':
			mode = rangeMode;
			Serial.println( "Range mode active" );
			succeeds = true;
			break;
		case 's':
			mode = statusMode;
			Serial.println( "Status mode active" );
			succeeds = true;
			break;
	}
	
	return succeeds;
}

// Called when data is requested by a master when it wants to read data
bool Commands::handleRequest() {
	switch ( mode ) {
		case initialMode:
			Wire.write( (uint8_t)0xAA );
			Wire.write( (uint8_t)0x55 );
			// Do not do Serial until after writes have been done
//			Serial.println( "In handleRequest in initialMode" );
			break;

		case statusMode:
			vIn = analogRead( V_IN_PIN );
			Wire.write( (uint8_t)((vIn & 0xFF00 ) >> 8) );
			Wire.write( (uint8_t)(vIn & 0xFF) );
			Wire.write( (uint8_t)((stateBits & 0xFF00 ) >> 8) );
			Wire.write( (uint8_t)(stateBits & 0xFF) );
//			status = (vIn & 0x0FFFF) || ( ( stateBits << 16 ) & 0xFFFF0000 );
//			Serial.println( "In handleRequest in statusMode" );
			break;
			
		case rangeMode:
//			Serial.println( "In handleRequest in rangeMode" );
//			memcpy( buffPtr, &status, 4);
			break;
			
		default:
//			Serial.println( "In handleRequest in unknown mode" );
			return false;
			break;
	}
	return true;
}
