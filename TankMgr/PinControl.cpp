//
// PinControl.cpp
// Library C++ code
// ----------------------------------
// Developed with embedXcode
// http://embedXcode.weebly.com
//
// Project 		vmdev
//
// Created by 	William Snook, 6/2/18 8:42 AM
// 				billsnook
//
// Copyright 	(c) William Snook, 2018
//


#include "PinControl.h"
#include "Servo.h"

Servo	scanner;

PinControl::PinControl() {
	
	ledState = LOW;				// ledState used to set the LED
	previousMillis = 0;			// will store last time LED was updated
	interval = 500;				// blink interval (milliseconds)

	isInitialized = false;
	isRunning = false;
	mainPowerState = false;
	rpiPowerState = false;
}

bool PinControl::setupPins() {
	
	pinMode( ledPin, OUTPUT );
	digitalWrite( ledPin, LOW );
	pinMode( MainPowerPin, OUTPUT );
	digitalWrite( MainPowerPin, LOW );
	pinMode( RPiPowerPin, OUTPUT );
	digitalWrite( RPiPowerPin, LOW );
//	pinMode( LidarDonePin, INPUT );
//
//	scanner.attach( ScannerPin );
//	setAngle( 90 );

	isInitialized = true;
	isRunning = false;      // No blinking at startup
    
	return true;
}

bool PinControl::resetPins() {
	
	isRunning = false;
	return true;
}

void PinControl::toggle() {
	
	if ( !isInitialized || !isRunning ) {
		digitalWrite( ledPin, LOW );
		return;
	}
	
	unsigned long currentMillis = millis();
	if ( ( currentMillis - previousMillis ) >= interval ) {
		previousMillis = currentMillis;	// save the last time we blinked the LED
		
		if (ledState == LOW) {
			ledState = HIGH;
		} else {
			ledState = LOW;
		}
		digitalWrite( ledPin, ledState );
	}
}

// Power management
void PinControl::mainPowerOff() {
	
	digitalWrite( MainPowerPin, LOW );
}

void PinControl::mainPowerOn() {
	
	digitalWrite( MainPowerPin, HIGH );
}

void PinControl::powerToggle() {
	
	if ( mainPowerState ) {
		mainPowerOff();
	} else {
		mainPowerOn();
	}
	mainPowerState = !mainPowerState;
}

void PinControl::piPowerOff() {
	
	digitalWrite( RPiPowerPin, LOW );
}

void PinControl::piPowerOn() {
	
	digitalWrite( RPiPowerPin, HIGH );
}
