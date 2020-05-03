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


#include "Pins.h"

#ifndef Commands_h
#define Commands_h


enum ControllerMode {
	initialMode = 0,
	statusMode,
	rangeMode,
	scanMode
};


class Commands {
	
public:
	explicit			Commands();

	long				status;		// Current status of this device
	ControllerMode		mode;

	int					stateBits;
	int 				vIn;
	
	int					last;
	int					next;
	int					range;

	bool 				parseI2CCommand( byte command, byte parameter );
	bool 				handleI2CRequest();
};

#endif // Commands_h
