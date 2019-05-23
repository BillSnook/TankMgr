//
//  Monitor.cpp
//  Index
//
//  Created by William Snook on 2/23/19.
//  Copyright © 2019 billsnook. All rights reserved.
//

#include "Monitor.h"
#include "PinControl.h"

extern	PinControl	pinControl;
/*
enum MonitorMode {
	unknownMode = 0,
	serialOnlyMode,
	lowExternalPowerMode,
	batteryNiCadPowerMode,
	batteryLiIonPowerMode,
	benchExternalPowerMode
};
*/

Monitor::Monitor() {
	
	checkIntervalMs = 0;
}

void Monitor::statusCheck() {	// Called to do periodic status checks

	unsigned long currentMs = millis();
	if ( ( currentMs - checkIntervalMs ) < 1000 ) {		// Only check about every second
		return;
	}
	checkIntervalMs = currentMs;

//	vIn = analogRead( V_IN_PIN );
//
//	// Test vIn against expected values to determine what our power situation is and set status accordingly
//	double voltage = ( vIn * 15 ) / 1024.0;
//	if ( voltage < 5.5 ) {				// No external vIn, running on USB serial +5v power
//		status = serialOnlyMode;
//	} else {
//		if ( voltage < 7.0 ) {							// TBD
//			status = lowExternalPowerMode;
//		} else {
//			if ( voltage < 8.2 ) {
//				status = batteryNiCadPowerMode;
//			} else {
//				if ( voltage < 9.5 ) {
//					status = batteryLiIonPowerMode;
//				} else {
//					status = benchExternalPowerMode;
//				}
//			}
//		}
//	}
//
//	switch ( status ) {					// Sets appropriate blink change interval == one half of cycle time
//		case serialOnlyMode:
//			pinControl.interval = 1000;		// 1 second
//			break;
//
//		case lowExternalPowerMode:
//			pinControl.interval = 100;		// 1/10 second - warning
//			break;
//
//		case batteryNiCadPowerMode:
//			pinControl.interval = 2000;		// 2 seconds
//			break;
//
//		case batteryLiIonPowerMode:
//			pinControl.interval = 4000;		// 4 seconds
//			break;
//
//		case benchExternalPowerMode:
//			pinControl.interval = 10000;		// 10 seconds
//			break;
//
//		default:
//			pinControl.interval = 1000;		// 1 second
//			break;
//	}

}
