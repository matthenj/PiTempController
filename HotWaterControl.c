

#include "DS18B20.h"
#include "HeaterController.h"
#include <stdio.h>
#include <unistd.h> /* Sleep */
#include <time.h>   /* Timing for records */

#define ENABLED_PIN 29u

#ifndef TEST_ENV
    #define SENSOR_ID_TOP "28-011927419e6a" /* TAPE */
    #define SENSOR_ID_BOTTOM "28-0119270d05f9" /* NO TAPE */
#else

    #define SENSOR_ID_TOP "28-011927419e6a" /* TAPE */
    #define SENSOR_ID_BOTTOM SENSOR_ID_TOP /* NO TAPE - only one sensor on test rig*/

#endif

/* char arrays used, as sensors accessed through path on disk */
static char TOP_OF_TANK[MAX_FILE_LENGTH];
static char BOTTOM_OF_TANK[MAX_FILE_LENGTH];

int main()
{

    Initialise(SENSOR_ID_TOP, TOP_OF_TANK, MAX_FILE_LENGTH);
    Initialise(SENSOR_ID_BOTTOM, BOTTOM_OF_TANK, MAX_FILE_LENGTH);
    

//#ifdef TEST_ENV
    printf("%d %d \n", GetRawValue(TOP_OF_TANK), GetRawValue(BOTTOM_OF_TANK));

//#endif

	InitialiseWaterHeater(ENABLED_PIN);
	CycleHeater();

}
