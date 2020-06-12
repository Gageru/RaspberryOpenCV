#include <wiringPi.h>
#include <cstdio>
//drv8825
#define LED6 0
#define  LED10 2
//a4898
#define  LED4 4
#define LED5 5
//vcc
#define LED7 7
int main (void) 
{ 
wiringPiSetup () ; //инициализируем библиотеку 
//pinMode (LED7, OUTPUT) ; 
pinMode (LED6, OUTPUT) ; //step
pinMode (LED10, OUTPUT) ; //dir
pinMode (LED4, OUTPUT) ; 
pinMode (LED5, OUTPUT) ; 

int step1 = 300;
int step2 = 300;
	//digitalWrite (LED7, 1);
	digitalWrite (LED10, 1);
	
	for(int i = 0 ; i < step1 ; i++) 
	{
		digitalWrite (LED6, 1);
		delayMicroseconds(1000);
		digitalWrite (LED6, 0);
		delayMicroseconds(1000);
		
	}
	/*digitalWrite (LED5, 1);
	for(int i = 0 ; i < step2 ; i++) 
	{
		digitalWrite (LED4, 1);
		delayMicroseconds(1000);
		digitalWrite (LED4, 0);
		delayMicroseconds(1000);
	}*/
	
	
return 0 ; 
}
