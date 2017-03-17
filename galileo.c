/*
 * File:   galileo.c    
 * Author: Danae Moss 
 *         
 * Simple Write code for Galileo interfaced with PIC16F18857
 *
 * Created on March 3, 2017, 6:15 PM
 */

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

    //Linux GPIO number // Aruino Shield Pin Name

#define Strobe     (40) // 8
#define GP_4       (48) // A0
#define GP_5	   (50) // A1
#define GP_6	   (52) // A2
#define GP_7	   (54) // A3
#define GPIO_DIRECTION_IN      (1)  
#define GPIO_DIRECTION_OUT     (0)
#define ERROR                  (-1)

//open GPIO, set the direction and value
int openGPIO(int gpio, int direction )
{
	//initialize char buffer for Linux commands
	char buffer[50];
	
     //   1.set the GPIO
	 sprintf(buffer, "echo %d >/sys/class/gpio/export", gpio);
	 system(buffer);

	//    2.set the direction
	if (direction) //if direction = in
	{
			sprintf(buffer,"echo in > /sys/class/gpio/gpio%d/direction", gpio);
	}
	else
	{
			sprintf(buffer,"echo out > /sys/class/gpio/gpio%d/direction", gpio);
	}
	system(buffer);

	//    3.set the voltage
	sprintf(buffer, "echo 1  > /sys/class/gpio/gpio%d/value", gpio);
	system(buffer);
    
}

//write 4-bit value to GPIO pins
int writeGPIO(unsigned int msg)
{
	int i;
	int bit;
	
	//GPIO pins arranged from least significant bit to most significant bit
	int pins[4] = {GP_4, GP_5, GP_6, GP_7};
	int value;
	
	for (i = 0; i < 4; i++)
	{
		value = (msg >> i) & 1; //get ith bit of msg
		writePin(pins[i], value); //write ith bit to GPIO pin
	}
}

//write value to single GPIO pin
int writePin(int pin, int value)
{
	char buffer[50];
	sprintf(buffer, "echo %d  > /sys/class/gpio/gpio%d/value", value, pin);
	system(buffer);
}

//main
int main(void)
{
 	  int fileHandleGPIO_4;
        int fileHandleGPIO_5;
        int fileHandleGPIO_6;
        int fileHandleGPIO_7;
	  int fileHandleGPIO_S; 
        fileHandleGPIO_4 = openGPIO(GP_4, GPIO_DIRECTION_OUT);
        fileHandleGPIO_5 = openGPIO(GP_5, GPIO_DIRECTION_OUT);
        fileHandleGPIO_6 = openGPIO(GP_6, GPIO_DIRECTION_OUT);
        fileHandleGPIO_7 = openGPIO(GP_7, GPIO_DIRECTION_OUT);
	  fileHandleGPIO_S = openGPIO(Strobe, GPIO_DIRECTION_OUT);
       
        while(1)
        {
              // 1.Strobe low
			  writePin(Strobe, 0);
			  
              // 2.write data
			  writeGPIO(0xd);
			  
              // 3.Strobe high
  			  writePin(Strobe, 1);
			  sleep(10);

	         //4.Strobe low
 			  writePin(Strobe, 0);

        }
}