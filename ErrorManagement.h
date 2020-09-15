/********************************************************************
*
*	Name: "ErrorMangement.h"
*	Author: njm
*
*	Description:	Header file to keep error codes in one place
*
*	Revision Hist:	08-09-2020 V1.0 - Codes Imported
*
********************************************************************/


#ifndef __ERROR_MANAGEMENT
#define __ERROR_MANAGEMENT

#define SUCCESS 0
#define HEATER_ON 1u
#define HEATER_OFF 0u
#define ERROR_CODE_START -195
#define FILE_NOT_FOUND -200
#define FILE_IO_ERROR -201
#define STORAGE_POINTER_NULL -202
#define STORAGE_POINTER_TO_SMALL -203
#define WATER_HEATER_ALREADY_INIT -204
#define WATER_HEATER_NOT_INIT -205
#define UNKNOWN_STATE -206


#define ABORT -240

#define LOG_BY_DEFAULT 1
#define LOGGING_ENABLED 1

//#define TEST_ENV
#define DEBUG


#endif /* __ERROR_MANAGEMENT */
