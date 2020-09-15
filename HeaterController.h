
/******************************************************************************
*
*   Name: "HeaterController.h"
*   Author: njm
*
*   Description:    Provides interface to heater relay. Control pin is set 
* 					on initialisation - this must be called first. Relay is then 
*                   controllable through SwitchHeaterOn/SwitchHeaterOff
*                   
*
*
*                   Written for UNIX
*
*   Revision Hist:  08-09-2020 V1.0 - Initial methods written.
*					15-09-2020 V1.1 - Enable/Disable Methods written. Comment
*									  added.
*
******************************************************************************/ 


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
int16_t SwitchHeaterOn(void);
int16_t SwitchHeaterOff(void);
uint8_t IsHeaterOn(void);



#endif




