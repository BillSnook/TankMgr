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
#include "PinControl.h"
#include "Commands.h"
#include "WireComm.h"
#include "Ultrasonic.h"

extern Ultrasonic ultrasonic;
extern PinControl pinControl;

bool testNewComm = true;

Commands::Commands() {
	
	pinMode( V_IN_PIN, INPUT );
	mode = initialMode;
	status = 0;
	stateBits = 0x4030;		// Test: @0
	last = 0;
	next = 0;
	range = 0;
//	vIn = analogRead( V_IN_PIN );
//	Serial.print("Commands initialization with vIn: ");
//	Serial.println( vIn );
    Serial.print("I2C slave address: ");
    Serial.println( I2C_SLAVE_ADDRESS );
}

// REMOTE commands (writes from the Pi) are handled here
// Called if it seems to be a command and returns true if command succeeds
bool Commands::parseCommand( byte command, byte parameter ) {
	int succeeds = false;
    int angleMs;
	switch ( command ) {		// Modes determine how read data is structured
        case 'p': {				// Ping - parameter is the angle
            Serial.println("parseCommand p");
			mode = rangeMode;
			angleMs = abs( parameter - next );	// Change of angle
			next = parameter;	// When next range is measured, this becomes an index
			pinControl.setAngle( parameter );
			angleMs *= 20;		// 20 ms x angle_to_be_moved
			delay( angleMs );	// Wait for angle to be reached
			ultrasonic.ranger( parameter );
			succeeds = true;
            break;
        }
        case 's':   			// Status
            Serial.println("parseCommand s");
			mode = statusMode;
			succeeds = true;
            break;
        case 'v': 				// Power relay enable
            Serial.println("parseCommand v");
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
bool Commands::handleRequest() {
	switch ( mode ) {
		case initialMode:
            if ( testNewComm ) {
                Wire.write( 2 );    // Count
            }
			Wire.write( (uint8_t)0xAA );
			Wire.write( (uint8_t)0x55 );
			break;

        case statusMode:
		case scanMode:
//			vIn = analogRead( V_IN_PIN );
            if ( testNewComm ) {
                Wire.write( 0x17 );    // Count
                Wire.write( (uint8_t)0xAA );
                Wire.write( (uint8_t)0x55 );
                Wire.write( (uint8_t)0xCC );
                Wire.write( (uint8_t)0xFF );
//                uint8_t block[8];
//                uint8_t *blk = block[0];
//                block[0] = (uint8_t)0x04;
//                block[1] = (uint8_t)0xAA;
//                block[2] = (uint8_t)0x55;
//                block[3] = (uint8_t)0xCC;
//                block[4] = (uint8_t)0xFF;
//                Wire.write(blk, 4);
            } else {
                Wire.write( (uint8_t)0x05 );
                Wire.write( (uint8_t)0x55 );
                Wire.write( (uint8_t)0xCC );
                Wire.write( (uint8_t)0xFF );
                Wire.write( (uint8_t)0x88 );
                Wire.write( (uint8_t)0x77 );
//                Wire.write( (uint8_t)((vIn >> 8) & 0xFF) );
//                Wire.write( (uint8_t)(vIn & 0xFF) );
//                Wire.write( (uint8_t)((stateBits >> 8) & 0xFF) );
//                Wire.write( (uint8_t)(stateBits & 0xFF) );
            }
			mode = statusMode;  // iff scanmode
//            Serial.println("Returned status");
			break;
			
		case rangeMode:
			last = ultrasonic.last;
			range = ultrasonic.range;
            if ( testNewComm ) {
                Wire.write( 4 );    // Count
                Wire.write( (uint8_t)0x84 );
                Wire.write( (uint8_t)0x21 );
                Wire.write( (uint8_t)0xF0 );
                Wire.write( (uint8_t)0xFF );
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
