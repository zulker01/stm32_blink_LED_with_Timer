#include "stm32f4xx.h"
#include "math.h"
// Device header

double sineWave(int degree)
	
{
		
	double wave1 = ( 
		2047 * sin(degree * 3.1416/180 ));
	
	wave1 = 2048 + wave1;
	return  wave1;
}


int main()
{
 	/*
	1. peripheral khujte hobe ,(DAC)
	2. peripheral er bus er clock enable korte hobe.(RCC diya arki )
	3. peripheral use korte hobe
	*/
	RCC->AHB1ENR |= 0x1;
	GPIOA->MODER |= 0x300;
		RCC->APB1ENR |= 0x20000000 ;	
	DAC->CR |= 0X1;
	int degree = 0;
	double wave1=2090;
	/*while( 1 )
	{
		degree++;
			if( degree == 361 )
				degree = 0;
		wave1 = sineWave(degree);
			DAC->DHR12R1 = wave1 ;
	}*/
	DAC->DHR12R1 = wave1 ;
	/*while(1)
	{
		degree++;
		if( degree ==361)
			degree = 0;
		
		if(degree >=0 && degree<=180)
			wave1 =4090;
		else
			wave1 = 0;
		DAC->DHR12R1 = wave1;
	}
	*/
	/*
	  int y = 2000;
	flag = 0;
	while(1)
	{
		degree++;
		if(flag ==0 )
				y++;
		else if(flag == 1)
		{
				y--;
		}
		if(y = 4090 )
			flag =1;
		else if( y= 0)
			flag = 0;
		DAC->DHR12R1 = y ;
		
	*/



}