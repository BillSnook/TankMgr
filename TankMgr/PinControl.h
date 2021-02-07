///
/// @file		PinControl.h
/// @author		William Snook
/// @author		billsnook
///
/// @date		6/2/18 8:42 AM
/// @copyright	(c) William Snook, 2018

#ifndef LEDs_h
#define LEDs_h

#include <Arduino.h>
#include "Pins.h"


class PinControl {
	
//	bool    		debug;
	int				ledPin =  LED_BUILTIN;		// the number of the LED pin
	int				ledState;					// ledState used to set the LED
	unsigned long	previousMillis;				// will store last time LED was updated

	bool			isInitialized;
	
	int				angle;						// Servo angle for scanner

public:
	bool			isRunning;
	unsigned long	interval;					// blink interval (milliseconds)
	boolean			mainPowerState;
	boolean			rpiPowerState;

	explicit PinControl();

	bool	setupPins();
	bool	resetPins();
	void	toggle();
	
	void 	mainPowerOff();
	void 	mainPowerOn();
	void	powerToggle();
	
	
	void 	piPowerOff();
	void 	piPowerOn();

};

#endif // LEDs_h
