#include "HeaterController.h"
#include <wiringPi.h>
#include <unistd.h> /* Sleep */
#include <stdio.h> /* debug printf */

#define READY 1
#define NOT_READY 0

#define HEATER_ON 1
#define HEATER_OFF 0


#define CYCLE_DELAY 30


static uint8_t enabled_pin = 0;
static uint8_t initialised = NOT_READY;


int16_t InitialiseWaterHeater(const uint8_t pin){

	if(initialised == 0)
	{
		wiringPiSetup();
		enabled_pin = pin;
		pinMode((int)enabled_pin, OUTPUT);
		initialised = READY;
		return SUCCESS;
	}

	return WATER_HEATER_ALREADY_INIT;

}

int16_t CycleHeater(void){

	if(initialised == NOT_READY){
		return WATER_HEATER_NOT_INIT;
	}


#ifdef DEBUG
	printf("Heater on...\n");
#endif

	digitalWrite(enabled_pin, HEATER_ON);
	sleep(CYCLE_DELAY);

#ifdef DEBUG
	printf("Heater off...\n");
#endif


	digitalWrite(enabled_pin, HEATER_OFF);
}


