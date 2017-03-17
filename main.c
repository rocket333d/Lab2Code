/*
 * File:   main.c
 * Author: Danae
 * 
 * Simple Read code for PIC16F18857 interfaced with Galileo
 *
 * Created on March 3, 2017, 4:50 PM
 */

#include <pic16f18857.h>
#include <xc.h>

//Sets the GPIO pins to input so they can be read
void set_receive()
{
   //1.set RA0 and RA2 as input
    TRISA = 0b00000101;
    
   //2.set RC2 RC3 RC4 RC5 as input
    TRISC = 0b00111100;
    
   //3.set them as digital I/O except RA0
    ANSELA = 0b00000001;
    ANSELC = 0;
  
}

//Reads message from GPIO via strobe protocol from Galileo
int receive_msg()
{
    set_receive();
    //1.wait strobe high
    while (PORTAbits.RA2 == 1);
    
    //2.wait strobe low
    while (PORTAbits.RA2 == 0);        
    
    //3.read the data
    int read_msg;
    read_msg = (PORTC >> 2) & 0xf; //shift right 2 & isolate bits 3-0
            
    //4.wait strobe high
    while (PORTAbits.RA2 == 1);
    
    //5.return the data
    return read_msg;
}

//Main function: Reads message from GPIO, if message is expected, lights LED
void main(void) {
    RC0 = 0;
    int msg;
    while(1)
    {  
		msg=receive_msg();
    
		if(msg == 0xd)
		{
			RC0 = 1;  //light the LED
		}
		else
		{
			RC0 = 0; //LED off
		}
    
    } 
}