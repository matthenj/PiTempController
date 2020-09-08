
CC=gcc
CFLAGS=-lwiringPi
DEPS =  ErrorManagement.h DS18B20.h HeaterController.h
OBJ = DS18B20.o HeaterController.o HotWaterControl.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

HotWaterController.ex: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
