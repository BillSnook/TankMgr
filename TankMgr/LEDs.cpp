//
// LEDs.cpp
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


#include "LEDs.h"


LEDs::LEDs() {
	
	ledState = LOW;				// ledState used to set the LED
	previousMillis = 0;			// will store last time LED was updated
	interval = 1000;			// blink interval (milliseconds)

	isInitialized = false;
	isRunning = false;
}

bool LEDs::setupForLEDs() {
	
	pinMode( ledPin, OUTPUT );
	digitalWrite( ledPin, LOW );
	pinMode( MainPowerPin, OUTPUT );
	digitalWrite( MainPowerPin, LOW );

	isInitialized = true;
	isRunning = true;
	return true;
}

bool LEDs::resetForLEDs() {
	
	isRunning = false;
	return true;
}

void LEDs::toggle() {
	
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
bool LEDs::mainPowerOff() {
	
	digitalWrite( MainPowerPin, LOW );
}

bool LEDs::mainPowerOn() {
	
	digitalWrite( MainPowerPin, HIGH );
}


