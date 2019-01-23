///
/// @file		Commands.h
/// @brief		Library header
/// @details	<#details#>
/// @n
/// @n @b		Project TankMgr
/// @n @a		Developed with [embedXcode](https://embedXcode.weebly.com)
///
/// @author		William Snook
/// @author		billsnook
///
/// @date		1/13/19 11:25 AM
/// @version	<#version#>
///
/// @copyright	(c) William Snook, 2019
/// @copyright	<#licence#>
///
/// @see		ReadMe.txt for references
///


#include "Arduino.h"

#ifndef Commands_h
#define Commands_h

#define V_IN_PIN	A0

enum ControllerMode {
	initialMode = 0,
	statusMode,
	rangeMode
};


class Commands {
	
public:
	explicit	Commands();

	long				status;		// Current status of this device
	ControllerMode		mode;

	long				stateBits;
	long 				vIn;

	bool parseCommand( unsigned char command );
	bool handleRequest();
};

#endif // Commands_h
