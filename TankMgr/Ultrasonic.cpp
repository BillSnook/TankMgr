//
//  Ultrasonic.cpp
//  Index
//
//  Created by William Snook on 6/8/18.
//  Copyright © 2018 billsnook. All rights reserved.
//

#include "Ultrasonic.h"
#include "Pins.h"


Ultrasonic::Ultrasonic() {
	isInitialized = false;
	isRunning = false;
	previousMillis = 0;
	interval = 1000;

	pinMode(TrigPin, OUTPUT);
	digitalWrite(TrigPin, LOW);
	pinMode(EchoPin, INPUT);

}

/*Begin the detection and get the pulse back signal*/
bool Ultrasonic::setupForUltrasonic() {
	
	isInitialized = true;
//	isRunning = true;
	
	return true;
}

void Ultrasonic::ranger( int parameter ) {
	
	next = parameter;
//	range = ping();
	range = pingSerialDisplay( parameter, false );	// Ping and show no results on Serial port
	last = parameter;
}

long Ultrasonic::ping(void) {
	
	digitalWrite(TrigPin, LOW);
	delayMicroseconds(2);
	digitalWrite(TrigPin, HIGH);
	delayMicroseconds(5);
	digitalWrite(TrigPin,LOW);
	duration = pulseIn(EchoPin, HIGH);
	return duration;
}

/*The measured distance from the range 0 to 400 Centimeters*/
long Ultrasonic::microsecondsToCentimeters(void) {
	
	return duration/29/2;
}

long Ultrasonic::microsecondsToMillimeters(void) {
	
	long decaDuration = duration * 10;
	return decaDuration/29/2;
}

/*The measured distance from the range 0 to 157 Inches*/
long Ultrasonic::microsecondsToInches(void) {
	
	return duration/74/2;
}

long Ultrasonic::pingReturnCentimeters() {
	
	ping();			// get the current signal time into duration
	return microsecondsToCentimeters();
}

void Ultrasonic::pingPeriodicDisplay() {

	if ( !isInitialized || !isRunning ) {
		return;
	}
	
	unsigned long currentMillis = millis();
	bool ready = ( ( currentMillis - previousMillis ) >= interval );
	if ( !ready ) {
		return;
	}
	previousMillis = currentMillis;	// save the last time we measured
	
	long cm = pingReturnCentimeters();
	Serial.print(duration);
	Serial.print("uS: ");
//	Serial.print(RangeInInches);		//0~157 inches
//	Serial.print(" inch, ");
	Serial.print(cm);	//0~400cm
	Serial.println(" cm");
}

long Ultrasonic::pingSerialDisplay( int inAngle, bool print ) {
	
	long result = ping();			// get the current signal time into duration

	if ( print ) {
		long cm = microsecondsToCentimeters();
		long mm = microsecondsToMillimeters();
		long in = microsecondsToInches();
		char angle[8];
		char pulse[8];
		
		sprintf( angle, "%3d", inAngle );
		sprintf( pulse, "%5l", result );
		Serial.print( angle );
        Serial.print("º: ");
		Serial.print( pulse );
		Serial.print( " uS, " );
		Serial.print( cm );	//0~400cm
		Serial.print(" cm, " );
		Serial.print( mm );	//0~4000mm
		Serial.print(" mm, " );
		Serial.print( in );	//0~120in
		Serial.println(" inches");
	}
	return result;
}
