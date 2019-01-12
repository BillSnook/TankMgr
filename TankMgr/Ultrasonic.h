//
//  Ultrasonic.h
//  Index
//
//  Created by William Snook on 6/8/18.
//  Copyright © 2018 billsnook. All rights reserved.
//

#ifndef Ultrasonic_h
#define Ultrasonic_h

#include <Arduino.h>


class Ultrasonic {
	
private:
	int				controlPin;			// Arduino pin connected with SIG pin of Ultrasonic
	unsigned long	previousMillis;		// will store last time LED was updated
	long			interval;			// blink interval (milliseconds)
	bool			isInitialized;
	
	
public:
	unsigned long	duration;			// the Pulse time received;
	bool			isRunning;

	explicit 		Ultrasonic();
	
	bool			setupForUltrasonic();
	void 			measure();
	long			makeMeasurement();
	
	long			processCycle( int index, bool print );

	long 			microsecondsToCentimeters(void);
	long 			microsecondsToInches(void);

private:
	void 			distanceMeasure(void);
};

#endif /* Ultrasonic_h */
