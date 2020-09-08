

/******************************************************************************
*
*   Name: "DS18B20.h"
*   Author: njm
*
*   Description:    File containing methods to interpret the output of the 
*                   DS18B20 sensor, and return it as double or raw. Sensor 
*                   ID is set in a init method.
*
*
*                   Written for UNIX
*
*   Revision Hist:  29-08-2020 V1.0 - Initial methods written.
*
******************************************************************************/ 

#ifndef _DS18B20
#define _DS18B20

#define SUCCESS 0
#define ERROR_CODE_START -195
#define FILE_NOT_FOUND -200
#define FILE_IO_ERROR -201
#define STORAGE_POINTER_NULL -202
#define STORAGE_POINTER_TO_SMALL -203

#define MAX_PATH 4096
#define MAX_FILE_LENGTH 255

#define TEST_ENV
#define DEBUG


#include <stdint.h>



/* Method sets "sensor id" to query for other menus*/
/* sensor_id :  the unique address of the sensor */
/* sensor_path : char* pointer with at least str_length allocated*/
/* str_length : length of sensor_path*/
/* returns - return code defined above. */
int16_t Initialise(const char * sensor_id, char * sensor_path, uint32_t str_length);



/* Get raw value */
int32_t GetRawValue(); 

/* Get current temperature in C. */
float   GetProcessedTemperatureInCelcius();

#endif //_DS18B20