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
#include "TankMgr.h"
#include "PinControl.h"
#include "Commands.h"
#include "WireComm.h"

extern PinControl pinControl;

bool testNewComm = false;

Commands::Commands() {
	
	pinMode( V_IN_PIN, INPUT );
	mode = initialMode;
	status = 0;
	stateBits = 0x4030;		// Test: @0
	last = 0;
	next = 0;
	range = 0;
//	vIn = analogRead( V_IN_PIN );
//#ifdef SERIAL_COMMANDS
//	Serial.print("Commands initialization with vIn: ");
//	Serial.println( vIn );

//    Serial.print("I2C slave address: ");
//    Serial.println( I2C_SLAVE_ADDRESS );
//#endif  // SERIAL_COMMANDS
}

// REMOTE commands (writes from the Pi) are handled here
// Called if it seems to be a command and returns true if command succeeds
bool Commands::parseI2CCommand( byte command, byte parameter ) {
	int succeeds = false;
    int angleMs;
	switch ( command ) {
        case 'p': {				// Ping - parameter is the angle
#ifdef SERIAL_COMMANDS
            Serial.println("parseI2CCommand p");
#endif  // SERIAL_COMMANDS
			mode = rangeMode;
			angleMs = abs( parameter - next );	// Change of angle
			next = parameter;	// When next range is measured, this becomes an index
//			pinControl.setAngle( parameter );
			angleMs *= 20;		// 20 ms x angle_to_be_moved
			delay( angleMs );	// Wait for angle to be reached
			succeeds = true;
            break;
        }
        case 's':   			// Status
#ifdef SERIAL_COMMANDS
            Serial.println("parseI2CCommand s");
#endif  // SERIAL_COMMANDS
			mode = statusMode;
			succeeds = true;
            break;
        case 'v': 				// Power relay enable
#ifdef SERIAL_COMMANDS
            Serial.println("parseI2CCommand v");
#endif  // SERIAL_COMMANDS
			if ( 0 == parameter ) {
				pinControl.mainPowerOff();
			} else {
				pinControl.mainPowerOn();
			}
			succeeds = true;
            break;
	}
	return succeeds;
}

// REMOTE requests (reads from the Pi) are handled here
// Called when data is requested by a master when it wants to read data
bool Commands::handleI2CRequest() {
	switch ( mode ) {
		case initialMode:
			Wire.write( (uint8_t)0xAA );
			Wire.write( (uint8_t)0x55 );
			break;

        case statusMode:
		case scanMode:
//			vIn = analogRead( V_IN_PIN );
            if ( testNewComm ) {
                Wire.write( (uint8_t)0xAA );
                Wire.write( (uint8_t)0x55 );
                Wire.write( (uint8_t)0xCC );
                Wire.write( (uint8_t)0x33 );
            } else {
                Wire.write( (uint8_t)((vIn >> 8) & 0xFF) );
                Wire.write( (uint8_t)(vIn & 0xFF) );
                Wire.write( (uint8_t)((stateBits >> 8) & 0xFF) );
                Wire.write( (uint8_t)(stateBits & 0xFF) );
            }
			mode = statusMode;  // iff scanmode
//            Serial.println("Returned status");
			break;
			
		case rangeMode:
            if ( testNewComm ) {
                Wire.write( (uint8_t)0x84 );
                Wire.write( (uint8_t)0x21 );
                Wire.write( (uint8_t)0xF0 );
                Wire.write( (uint8_t)0x0F );
            } else {
                Wire.write( (uint8_t)((last >> 8) & 0xFF) );
                Wire.write( (uint8_t)(last & 0xFF) );
                Wire.write( (uint8_t)((range >> 8) & 0xFF) );
                Wire.write( (uint8_t)(range & 0xFF) );
            }
			break;
			
		default:
			return false;
			break;
	}
	return true;
}
