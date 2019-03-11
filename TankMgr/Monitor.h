//
//  Monitor.h
//  Index
//
//  Created by William Snook on 2/23/19.
//  Copyright © 2019 billsnook. All rights reserved.
//

#ifndef Monitor_h
#define Monitor_h

#include "Pins.h"
#include <stdio.h>


enum MonitorMode {
	unknownMode = 0,
	serialOnlyMode,
	lowExternalPowerMode,
	batteryNiCadPowerMode,
	batteryLiIonPowerMode,
	benchExternalPowerMode
};


class Monitor {
	
	unsigned long	checkIntervalMs;	// will store last time for timed checks
	int				vIn;				// Most recent voltage measurement of vIn on Arduino

public:
	MonitorMode		status;				// Current status of this device

	
	explicit		Monitor();
	
	void			statusCheck();		// Called to do periodic status checks
};

#endif /* Monitor_h */
