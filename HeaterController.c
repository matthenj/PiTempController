/******************************************************************************
*
*   Name: "HeaterController.c"
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

#include "HeaterController.h"
#include <wiringPi.h>
#include <unistd.h> /* Sleep */
#include <stdio.h> /* debug printf */

#define READY 1
#define NOT_READY 0

#define CYCLE_DELAY 30

static int16_t SetHeaterState(uint8_t state);
static uint8_t enabled_pin = 0;
static uint8_t initialised = NOT_READY;
static uint8_t is_heater_on = HEATER_OFF;


int16_t InitialiseWaterHeater(const uint8_t pin)
{

	if(initialised == 0)
	{
		wiringPiSetup();
		enabled_pin = pin;
		pinMode((int)enabled_pin, OUTPUT);
		initialised = READY;
		SwitchHeaterOff();
		return SUCCESS;
	}
	else 
	{
		return WATER_HEATER_ALREADY_INIT;
	}
}

int16_t CycleHeater(void)
{

	if(initialised == NOT_READY)
	{
		return WATER_HEATER_NOT_INIT;
	}


#ifdef DEBUG
	printf("Heater on...\n");
#endif

	SetHeaterState(HEATER_ON);
	sleep(CYCLE_DELAY);

#ifdef DEBUG
	printf("Heater off...\n");
#endif

	SetHeaterState(HEATER_OFF);
}

static int16_t SetHeaterState(uint8_t state)
{
	int16_t return_code = SUCCESS; 

	if(initialised == NOT_READY)
	{
		return_code = WATER_HEATER_NOT_INIT;
	} 
	else if(state = HEATER_ON)
	{
		digitalWrite(enabled_pin, HEATER_ON);
		is_heater_on = HEATER_ON;
		return_code = SUCCESS;
	}
	else if(state = HEATER_OFF)
	{
		digitalWrite(enabled_pin, HEATER_OFF);
		is_heater_on = HEATER_OFF;
		return_code = SUCCESS;
	}
	else 
	{
		return_code = UNKNOWN_STATE;
	}

	return return_code;
}


int16_t SwitchHeaterOn(void)
{
	return SetHeaterState(HEATER_ON);
}

int16_t SwitchHeaterOff(void)
{
	return SetHeaterState(HEATER_OFF);
}

uint8_t IsHeaterOn(void)
{
	return is_heater_on;
}