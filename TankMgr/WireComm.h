///
/// @file		WireComm.h
/// @brief		Library header
/// @details	<#details#>
/// @n
/// @n @b		Project TankMgr
/// @n @a		Developed with [embedXcode](http://embedXcode.weebly.com)
///
/// @author		William Snook
/// @author		billsnook
///
/// @date		1/5/19
///
/// @copyright	(c) William Snook, 2019
///

#include <Wire.h>

#ifndef WireComm_h
#define WireComm_h

#define I2C_SLAVE_ADDRESS    0x08    // Needs to match value in cweb mtrctl app on Raspberry Pi


class WireComm {
	
	int				wireAddress;	// the address of i2c if slave
	bool			isSetup;		// can only be setup once

public:
	bool			isRunning;

	explicit		WireComm();

	void			setupForWireComm( bool beMaster );
	void			resetForWireComm();

	void			runWireComm();
	
	static void		requestEvent();
	static void		receiveEvent( int howMany );

};

#endif // WireComm_h
