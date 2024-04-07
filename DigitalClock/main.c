/*
 * main.c
 *
 *  Created on: Sep 20, 2023
 *      Author: Mohamed Abdelmawgoud
 */
#include <avr/io.h>
#include <avr/interrupt.h>

#define SET(REG,bit) REG |= ((1)<<bit)
#define CLEAR(REG,bit) REG &= ~((1)<<bit)
#define TOGGLE(REG,bit) REG ^= ((1)<<bit)
#define GET(REG,bit) ((REG)&((1)<<(bit)))
/*
 * intitializng time variables.
 */
volatile int seconds = 0 ;
volatile int minutes =27;
volatile int hours  = 1;
//********************************************

ISR(TIMER1_OVF_vect)
{
	static int ctr =  0 ;
	ctr++ ;
	if(ctr == 250)
	{
		seconds++ ;
		ctr = 0;
	}
	if(seconds == 60)
	{
		minutes++;
		seconds = 0 ;
	}
	if(minutes == 60)
	{

		hours++;
		minutes = 0 ;
	}
	if(hours == 24)
	{
		hours = 0 ;
		seconds = 0;
		minutes = 0 ;
		ctr = 0 ;
	}


	TCNT1 = 0xff83 ;

	SET(TIFR,TOV1);
}


int main()
{
	/*
	 * Setting 7segments ports as output.
	 */
	DDRC = 0xff ; //seconds display.
	DDRD = 0xff ; //minutes display.
	DDRA = 0xff ; //hours display.
//********************************************************

	/*
	 * enabling global and timer1 interrupts.
	 */
	SET(SREG,7);
	SET(TIMSK,TOIE1);
//********************************************************

	/*
	 * choosing the normal mode and prescaling to 256.
	 */
	TCCR1A = 0x00 ;
	TCCR1B = 0x04 ;
//********************************************************


	/*
	 * Setting initial value in timer1 counter.
	 */

	TCNT1 = 0xff83 ;

	while(1)
	{
		if(seconds < 10)
		{
			PORTC = (seconds);
		}
		else
		{
			PORTC = ((seconds/10)<<4) | (seconds%10);
		}



		if(minutes < 10)
		{
			PORTD = (minutes);
		}
		else
		{
			PORTD = ((minutes/10)<<4) | (minutes%10);
		}


		if(hours < 10)
		{
			PORTA = (hours);
		}
		else
		{
			PORTA = ((hours/10)<<4) | (hours%10);
		}
	}



	return 0 ;
}
