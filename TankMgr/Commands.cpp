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
int Commands::handleRequest( unsigned char *buffPtr ) {
	switch ( mode ) {
		case initialMode:
			Serial.println( "In handleRequest in initialMode" );

			break;

		case statusMode:
			Serial.println( "In handleRequest in statusMode" );
			vIn = analogRead( V_IN_PIN );
			status = (vIn && 0xFFFF) || ( stateBits << 16 );
			buffPtr[0] = 'W'; // (vIn && 0xFF00 ) >> 8;
			buffPtr[1] = 'o'; // vIn && 0xFF;
			buffPtr[2] = 'o'; // (stateBits && 0xFF00 ) >> 8;
			buffPtr[3] = 't'; // stateBits && 0xFF;
			buffPtr[4] = 0;
			
			const char outBuff[64] = {0};
			sprintf( outBuff, "vIn: 0x%02X%02X, state: 0x%02X%02X", buffPtr[0], buffPtr[1], buffPtr[2], buffPtr[3] );
			Serial.println( outBuff );
			sprintf( outBuff, "vIn: 0x%04X, state: 0x%04X", vIn, stateBits );
			Serial.println( outBuff );
			free( outBuff );

			return 4;
			break;
			
		case rangeMode:
			Serial.println( "In handleRequest in rangeMode" );
			memcpy( buffPtr, &status, 4);
			break;
			
		default:
			Serial.println( "In handleRequest in unknown mode" );
			break;
	}
	return 4;
}
