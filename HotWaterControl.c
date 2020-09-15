/******************************************************************************
*
*   Name: "HotWaterControl.c"
*   Author: njm
*
*   Description:    Provides main features of application. Main method may be
*                   moved out in due course. ToDo: Logging to file. Setting
*                   options via arguements. Testing. 
*
*
*                   Written for UNIX
*
*   Revision Hist:  08-09-2020 V1.0 - Initial methods written.
*					15-09-2020 V1.1 - CheckError, RunForDuration, HeatAndHold
*                                   and RunToMinimumSetpoint drafted.
*
******************************************************************************/ 

#include "DS18B20.h"
#include "HeaterController.h"
#include <stdio.h>
#include <unistd.h> /* Sleep */
#include <time.h>   /* Timing for records */

#define ENABLED_PIN 29u
#define REACTION_TIME 30
#define PRECISION 2000 //Units == thousands of a degree.  

#ifndef TEST_ENV
    #define SENSOR_ID_TOP "28-011927419e6a" /* TAPE */
    #define SENSOR_ID_BOTTOM "28-0119270d05f9" /* NO TAPE */
#else

    #define SENSOR_ID_TOP "28-011927419e6a" /* TAPE */
    #define SENSOR_ID_BOTTOM SENSOR_ID_TOP /* NO TAPE - only one sensor on test rig*/

#endif

/* char arrays used, as sensors accessed through path on disk */
static char TOP_OF_TANK_ADDRESS[MAX_FILE_LENGTH];
static char BOTTOM_OF_TANK_ADDRESS[MAX_FILE_LENGTH];
static unsigned long HEAT_LOOPS = 0; 
static uint32_t ENABLE_TEMPERATURE_LOGGING = LOG_BY_DEFAULT; //1 for logging per loop, 0 for no logging.  

int16_t CheckError(int32_t sensor_a, int32_t sensor_b)
{

    int16_t rtn_code = SUCCESS;

    if(sensor_a < ERROR_CODE_START)
    {
        printf("ERROR, ABORT Sensor A (Top) - Code %d \n", sensor_a);
        rtn_code = ABORT;
    }

    if(sensor_b < ERROR_CODE_START)
    {
        printf("ERROR, ABORT Sensor B (Bottom) - Code %d \n", sensor_b);
        rtn_code = ABORT;
    }

    return rtn_code;

}

/* Method switches on heater for a specified number of seconds, then switches it off */
void RunForDuration(const uint32_t seconds)
{

    int16_t rtn_code = SwitchHeaterOn();

    if(rtn_code != SUCCESS)
    {
        printf("Heater could not be switched on. Code %d\n", rtn_code);
        return;
    }

    sleep(seconds);
    
    rtn_code = SwitchHeaterOff();

    if(rtn_code != SUCCESS)
    {
        printf("Heater could not be switched off. Code %d\n", rtn_code);
        return;
    }

}

/*  Method will run heater until minimum tank temperature of "SetPoint" is reached. 
    Queries both High and Low Sensors. Set point should be in thousandsths of a degree:
    16C = 16000. This matches the raw value of the sensor. */
int32_t RunToMinimumSetPoint(const int32_t set_point, unsigned long * loops_performed)
{
    //If heat_loops is null, use global counter; 
    if(heat_loops == (void*)0)
    {
        loops_performed = &HEAT_LOOPS; 
    }


    int32_t bottom_current_temp = GetRawValue(BOTTOM_OF_TANK_ADDRESS);
    int32_t top_current_temp = GetRawValue(TOP_OF_TANK_ADDRESS);

    if(CheckError(top_current_temp, bottom_current_temp) == ABORT)
    {
        return ABORT; 
    }

    int16_t error_code = SUCCESS; 

    while(bottom_current_temp < set_point && top_current_temp < set_point)
    {
        uint8_t is_heater_on = IsHeaterOn();
   
        if(is_heater_on == HEATER_OFF)
        {
            error_code = SwitchHeaterOn();
            if(error_code != SUCCESS)
            {
                printf("Couldn't switch on heater, Code: %d \n", error_code);
                return ABORT;
            }
        }

        sleep(REACTION_TIME);        
        
        
        bottom_current_temp = GetRawValue(BOTTOM_OF_TANK_ADDRESS);
        top_current_temp = GetRawValue(TOP_OF_TANK_ADDRESS);

        if(CheckError(top_current_temp, bottom_current_temp) == ABORT)
        {
            error_code = SwitchHeaterOff();
            if(error_code != SUCCESS)
            {
                printf("Couldn't switch off heater, Code: %d \n", error_code);
            }
            return ABORT; 
        }

        *loops_performed += 1l; //Keep a record of approximate duration switched on. 

        if(ENABLE_TEMPERATURE_LOGGING == LOGGING_ENABLED)
        {
            //TODO: Write to output log file. 
        }

    }
    
    if(is_heater_on() == HEATER_ON){
        error_code = SwitchHeaterOff();
        if(error_code != SUCCESS)
        {
            printf("Couldn't switch off heater, Code: %d \n", error_code);
            return ABORT;            
        }

    }

    return SUCCESS;
}

/* Designed to Heat and Hold Temperature - Purge Tank of Bacteria.  */
int32_t HeatAndHold(int32_t min_set_point, uint32_t hold_time_seconds)
{

    int32_t upper_limit = min_set_point + PRECISION; 
    int32_t rtn_code = RunToMinimumSetPoint(upper_limit); //Heat Tank To over minimum Set point
    uint32_t iterator = 0; 
    
    if(rtn_code == ABORT)
    {
        printf("Couldn't reach setpoint.\n");
        return ABORT;
    }

    //Number of loops required to hold tank temperature for desired time.
    uint32_t target_loops = hold_time_seconds / REACTION_TIME; 
    
    //Check for rounding errors, and account for. 
    target_loops = (hold_time_seconds % REACTION_TIME) != 0 ? target_loops + 1: target_loops;
    
    for(iterator = 0; iterator < target_loops; iterator++)
    {
        int32_t bottom_current_temp = GetRawValue(BOTTOM_OF_TANK_ADDRESS);
        int32_t top_current_temp = GetRawValue(TOP_OF_TANK_ADDRESS);

        if(CheckError(top_current_temp, bottom_current_temp) == ABORT)
        {
            printf("Temperature Hold Failed\n");
            return ABORT; 
        }
    
        if(bottom_current_temp <= min_set_point || top_current_temp <= min_set_point)
        {   
            unsigned long loops_performed = 0; 
            rtn_code = RunToMinimumSetPoint(upper_limit, &loops_performed);
            if(rtn_code == ABORT)
            {
                printf("Couldn't hold setpoint.\n");
                return ABORT;
            }

            iterator += (uint32_t)loops_performed; //Assume temperature never fell too far below setpoint. Check logs to verify.            
        }
        else 
        {
            sleep(REACTION_TIME); 
        }
    
    }

    return SUCCESS;

}


int main()
{

    Initialise(SENSOR_ID_TOP, TOP_OF_TANK_ADDRESS, MAX_FILE_LENGTH);
    Initialise(SENSOR_ID_BOTTOM, BOTTOM_OF_TANK_ADDRESS, MAX_FILE_LENGTH);
	InitialiseWaterHeater(ENABLED_PIN);
    

//#ifdef TEST_ENV
    printf("%d %d \n", GetRawValue(TOP_OF_TANK), GetRawValue(BOTTOM_OF_TANK));

//#endif

	CycleHeater();

}
