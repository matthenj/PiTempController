

#ifndef __HEATER_CONTROLLER
#define __HEATER_CONTROLLER

#include "ErrorManagement.h"
#include "stdint.h"


/*
	Method Inistialises given pin to "out" and insures it is set to 0V.
	pin - pin connected to heater relay.

	return error code (See ErrorManagement.h)

*/
int16_t InitialiseWaterHeater(const uint8_t pin);

/*

	If initialised, switch heater on for 30 seconds, then off.

*/
int16_t CycleHeater(void);




#endif




