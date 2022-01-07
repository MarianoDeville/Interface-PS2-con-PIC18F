/*
high level delay routines - see delay.h for more info.
Designed by Shane Tolmie of KeyGhost corporation.  Freely distributable.
Questions and comments to shane@keyghost.com
PICuWEB - Program PIC micros with C. Site has FAQ and sample source code. http://www.workingtex.com/htpic
For Microchip 12C67x, 16C7x, 16F87x and Hi-Tech C
*/

#ifndef __DELAY_C
#define __DELAY_C
#include	"delay.h"

unsigned char delayus_variable;

void DelayBigUs(unsigned int cnt)
{
	unsigned char	i;
	i = (unsigned char)(cnt>>8);
	while(i>=1)
	{
		i--;
		DelayUs(253);
		CLRWDT();
	}
	DelayUs((unsigned char)(cnt & 0xFF));
}

//#pragma interrupt_level 1
void DelayMs(unsigned char cnt)
{
	unsigned char	i;
	do {
		i = 4;
		do {
			DelayUs(250);
			CLRWDT();
		} while(--i);
	} while(--cnt);
}

//this copy is for the interrupt function
void DelayMs_interrupt(unsigned char cnt)
{
	unsigned char	i;
	do {
		i = 4;
		do {
			DelayUs(250);
		} while(--i);
	} while(--cnt);
}

void DelayBigMs(unsigned int cnt)
{
	unsigned char	i;
	do {
		i = 4;
		do {
			DelayUs(250);
			CLRWDT();
		} while(--i);
	} while(--cnt);
}

void DelayS(unsigned char cnt)
{
	unsigned char i;
	do {
		i = 4;
		do {
			DelayMs(250);
			CLRWDT();
		} while(--i);
	} while(--cnt);
}

#endif


