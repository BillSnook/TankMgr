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
	interval = 1000;			// blink interval (milliseconds)

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

	scanner.attach( ScannerPin );
	scanner.write( 90 );

	isInitialized = true;
	isRunning = true;
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
bool PinControl::mainPowerOff() {
	
	digitalWrite( MainPowerPin, LOW );
}

bool PinControl::mainPowerOn() {
	
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

bool PinControl::piPowerOff() {
	
	digitalWrite( RPiPowerPin, LOW );
}

bool PinControl::piPowerOn() {
	
	digitalWrite( RPiPowerPin, HIGH );
}

void PinControl::setAngle( int angle ) {
	
	scanner.write( angle );
}
