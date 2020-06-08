#include <wiringPi.h>
#include <cstdio>
#define LED5 5
#define  LED4 4
int main (void) 
{ 
wiringPiSetup () ; //инициализируем библиотеку 
pinMode (LED4, OUTPUT) ; //step
pinMode (LED5, OUTPUT) ; //dir
int step = 9000;
	digitalWrite (LED5, 1);
	for(int i = 0 ; i < step ; i++) 
	{
		digitalWrite (LED4, 1);
		delayMicroseconds(1000);
		digitalWrite (LED4, 0); 
		delayMicroseconds(1000);
		
	}
	digitalWrite (LED5, 0);
	for(int i = 0 ; i < step ; i++) 
	{
		digitalWrite (LED4, 1); 
		delayMicroseconds(1000);
		digitalWrite (LED4, 0);
		delayMicroseconds(1000);
	}
return 0 ; 
}
