//
//  Ultrasonic.cpp
//  Index
//
//  Created by William Snook on 6/8/18.
//  Copyright © 2018 billsnook. All rights reserved.
//

#include "Ultrasonic.h"

//#define PulseDetectPin			7
#define PulsePin					7		// Pulse control pin for 3-pin ultrasonic range detector
#define DetectPin					8

Ultrasonic::Ultrasonic() {
	isInitialized = false;
	isRunning = false;
	previousMillis = 0;
	interval = 1000;

	pinMode(PulsePin, OUTPUT);
	digitalWrite(PulsePin, LOW);
	pinMode(DetectPin, INPUT);

}

/*Begin the detection and get the pulse back signal*/
bool Ultrasonic::setupForUltrasonic() {
	
	isInitialized = true;
//	isRunning = true;
	
	return true;
}

void Ultrasonic::distanceMeasure(void) {
	
	digitalWrite(PulsePin, LOW);
	delayMicroseconds(2);
	digitalWrite(PulsePin, HIGH);
	delayMicroseconds(5);
	digitalWrite(PulsePin,LOW);
	duration = pulseIn(DetectPin, HIGH);
}

/*The measured distance from the range 0 to 400 Centimeters*/
long Ultrasonic::microsecondsToCentimeters(void) {
	
	return duration/29/2;
}
/*The measured distance from the range 0 to 157 Inches*/
long Ultrasonic::microsecondsToInches(void) {
	
	return duration/74/2;
}

void Ultrasonic::measure() {

	if ( !isInitialized || !isRunning ) {
		return;
	}
	
	unsigned long currentMillis = millis();
	bool ready = ( ( currentMillis - previousMillis ) >= interval );
	if ( !ready ) {
		return;
	}
	previousMillis = currentMillis;	// save the last time we measured
	
	long cm = makeMeasurement();
	Serial.print(duration);
	Serial.print("uS: ");
//	Serial.print(RangeInInches);		//0~157 inches
//	Serial.print(" inch, ");
	Serial.print(cm);	//0~400cm
	Serial.println(" cm");
}

long Ultrasonic::makeMeasurement() {
	
	distanceMeasure();			// get the current signal time into duration
//	long RangeInInches = microsecondsToInches();//convert the time to inches;
//	long RangeInCentimeters = microsecondsToCentimeters();	//convert the time to centimeters
	return microsecondsToCentimeters();
}

long Ultrasonic::processCycle( int index, bool print ) {
	
	distanceMeasure();			// get the current signal time into duration

	long cm = microsecondsToCentimeters();
	long in = microsecondsToInches();

//	long diff;
	char angle[8];
	char pulse[8];
//	diffManager.currentRecord = 0;
//	diff = diffManager.put( index, cm );

	if ( print ) {
		sprintf( pulse, "%5u", duration );
		sprintf( angle, "%3d", index * 10);
		Serial.print( pulse );
		Serial.print( " uS, " );
		Serial.print( angle );
		Serial.print("º: ");
		Serial.print( cm );	//0~400cm
		Serial.print( " " );	//0~300cm
//		Serial.print( diff );	//0~300cm
//		Serial.print(" cm, " );	//0~300cm
		Serial.print( in );	//0~120in
		Serial.println(" inches");
	}
	

}
