///
/// @file		LEDs.h
/// @author		William Snook
/// @author		billsnook
///
/// @date		6/2/18 8:42 AM
/// @copyright	(c) William Snook, 2018

#ifndef LEDs_h
#define LEDs_h

#include <Arduino.h>

#define MainPowerPin	12


class LEDs {
	
//	bool    		debug;
	int				ledPin =  LED_BUILTIN;		// the number of the LED pin
	int				ledState;					// ledState used to set the LED
	unsigned long	previousMillis;				// will store last time LED was updated

	bool			isInitialized;

public:
	bool			isRunning;
	long			interval;					// blink interval (milliseconds)
	boolean			mainPowerState;

	explicit LEDs();

	bool	setupForLEDs();
	bool	resetForLEDs();

	void	toggle();
	
	bool 	mainPowerOff();
	bool 	mainPowerOn();
	void	powerToggle();

};

#endif // LEDs_h
