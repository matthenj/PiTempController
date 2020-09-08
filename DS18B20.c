
/******************************************************************************
*
*   Name: "DS18B20.c"
*   Author: njm
*
*   Description:    File containing methods to interpret the output of the 
*                   DS18B20 sensor, and return it as double or raw. Sensor 
*                   ID will be set in a init method.
*
*
*                   Written for UNIX
*
*   Revision Hist:  29-08-2020 V1.0 - Initial methods written.
*
******************************************************************************/ 


#include "DS18B20.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>



#define SENSOR_RESOLUTION 5
#define MARKER 't'
#define DIVISOR 1000.0f


#ifdef TEST_ENV
    #define PATH_TO_SENSOR "/home/njm/scratch/w1_bus_sim/"
#else 
    #define PATH_TO_SENSOR "/sys/devices/w1_bus_master1/"
#endif 

#define BUS_NAME "/w1_slave"


char SENSOR_ADDRESS[MAX_PATH];
char FILE_CONTENTS[MAX_FILE_LENGTH + 1];


int16_t FileExists(char * sensor_path)
{
    
    uint16_t rtn_code; 
    FILE * fp; 
    fp = fopen(sensor_path, "r");

    if(fp == NULL)
    {
        rtn_code = FILE_NOT_FOUND;  
    }
    else
    {
        int file_close_success= fclose(fp);
        rtn_code = (file_close_success == 0) ? SUCCESS : FILE_IO_ERROR;
    } 
    return rtn_code; 
}


int16_t Initialise(const char * sensor_id, char * sensor_path, uint32_t str_length)
{

    if(sensor_path == NULL)
    {
        return STORAGE_POINTER_NULL;
    }

    uint32_t total_string_length = strlen(sensor_id) + strlen(PATH_TO_SENSOR) + strlen(BUS_NAME);

    if(str_length <= total_string_length)
    {
        return STORAGE_POINTER_TO_SMALL;
    }

    sensor_path[0] = '\0';
    strcat(sensor_path, PATH_TO_SENSOR);
    strcat(sensor_path, sensor_id);
    strcat(sensor_path, BUS_NAME);

#ifdef DEBUG
    printf("Path to sensor: %s\n", sensor_path);
#endif 
    
    int16_t file_exists = FileExists(sensor_path);

    if(file_exists != SUCCESS)
    {
         sensor_path[0] = '\0';
    } 

    return file_exists;
    
}

int32_t GetRawValue(const char * sensor_address)
{

    FILE * fp; 
    fp = fopen(sensor_address, "r");
    

    if(fp == NULL)
    {
#ifdef DEBUG
        printf("File not found: %s.\n", sensor_address);
#endif 
        return FILE_NOT_FOUND;
    }
    
    /* Only care about last line */

    char * data_start; 
    char * success = 0;


    /* Search file for 't' - this indicates the RAW temperature value */
    do
    {
        success = fgets(FILE_CONTENTS, MAX_FILE_LENGTH, fp);
        data_start = strchr(FILE_CONTENTS, MARKER);

#ifdef DEBUG
    printf("data_start: %s \n", data_start);
#endif

    }
    while(data_start == NULL && success != NULL );


    int file_close_success= fclose(fp);
    if(file_close_success != 0 || success == NULL)
    {
#ifdef DEBUG
        printf("Can't close file.\n");
#endif 
        return FILE_IO_ERROR;
    }


    /* Temperature should be last 5 characters of the file */
    size_t contents_length = strlen(data_start);
    long raw_data = FILE_IO_ERROR;
    if(contents_length >= SENSOR_RESOLUTION)
    {
        /* advance ptr beyond t= */
        char * initial_character = data_start+2;
        for(int i = 0; i < SENSOR_RESOLUTION;i++)
        {
            if(!isdigit(*(initial_character+i)))
            {
#ifdef DEBUG
                printf("Can't parse data: %s\n", FILE_CONTENTS);
#endif 
                return FILE_IO_ERROR;
            }
        }

        char * end_data; 
        raw_data = strtol(initial_character, &end_data, 10);

#ifdef DEBUG
        printf("%ld \n", raw_data);
#endif
    }

    return (int32_t)raw_data; 
} 

float GetProcessedTemperatureInCelcius(const char * sensor_address)
{

    int32_t raw_data = GetRawValue(sensor_address);
    float converted_data = (float)raw_data / DIVISOR;
    return converted_data;

}



