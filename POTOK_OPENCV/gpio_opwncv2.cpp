#include <wiringPi.h>
#include <cstdio>
//drv8825
#define LED6 0
#define  LED10 2
//a4898
#define LED5 5
#define  LED4 4
int main (void) 
{ 
wiringPiSetup () ; //инициализируем библиотеку 
pinMode (LED6, OUTPUT) ; //step
pinMode (LED10, OUTPUT) ; //dir
pinMode (LED4, OUTPUT) ; 
pinMode (LED5, OUTPUT) ; 
int step1 = 600;
int step2 = 300;
	digitalWrite (LED10, 1);
	digitalWrite (LED5, 1);
	for(int i = 0 ; i < step1 ; i++) 
	{
		if (i <= step2){
		digitalWrite (LED6, 1);
		}
		digitalWrite (LED4, 1);
		delayMicroseconds(1000);
		if (i <= step2){
		digitalWrite (LED6, 0);
		} 
		digitalWrite (LED4, 0);
		delayMicroseconds(1000);
		
	}
	digitalWrite (LED10, 0);
	digitalWrite (LED5, 0);
	for(int i = 0 ; i < step1 ; i++) 
	{
		if (i <= step2){
		digitalWrite (LED6, 1);
		}
		digitalWrite (LED4, 1);
		delayMicroseconds(1000);
		if (i <= step2){
		digitalWrite (LED6, 0);
		}
		digitalWrite (LED4, 0);
		delayMicroseconds(1000);
	}
	
	
return 0 ; 
}
