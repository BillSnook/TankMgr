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
	range = pingSerialDisplay( parameter, true );	// Ping and show results to Serial port
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
//	long RangeInInches = microsecondsToInches();//convert the time to inches;
//	long RangeInCentimeters = microsecondsToCentimeters();	//convert the time to centimeters
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

long Ultrasonic::pingSerialDisplay( int index, bool print ) {
	
	long result = ping();			// get the current signal time into duration

	long cm = microsecondsToCentimeters();
	long mm = microsecondsToMillimeters();
	long in = microsecondsToInches();

//	long diff;
//	diffManager.currentRecord = 0;
//	diff = diffManager.put( index, cm );

	if ( print ) {
		char angle[8];
		char pulse[8];
		sprintf( pulse, "%5u", duration );
		sprintf( angle, "%3d", index );
		Serial.print( pulse );
		Serial.print( " uS, " );
		Serial.print( angle );
		Serial.print("º: ");
		Serial.print( cm );	//0~400cm
		Serial.print(" cm, " );	//0~300cm
		Serial.print( mm );	//0~400mcm
		Serial.print(" mm, " );	//0~300cm
		Serial.print( in );	//0~120in
		Serial.println(" inches");
	}
	return result;
}
