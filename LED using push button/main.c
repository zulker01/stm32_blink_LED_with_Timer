#include "stm32f446xx.h"

int main(){
		RCC->AHB1ENR |= 0x1;// enable GPIOA
		GPIOA->MODER |= 0x400; //enable port PA5 as output 
		
		RCC->AHB1ENR |= 0x4;// enable GPIOC
	
		GPIOC->MODER &= ~GPIO_MODER_MODE13;
//		GPIOC->MODER |= 0x4000000; //enable port PC13 as input
		int test = 0;
		while(1){	
	
//		test = GPIOC->IDR;
//		test &=0x2000;
		//By default PC13 is high because push btn is connected to pull up register.
		//so when we press the btn it becomes low.
		if(GPIOC->IDR & 0x2000) //if PC13 is high
		{
				GPIOA->BSRR |=0x200000 ; // reset the PA5 pin (LED off )
		}	
		else
		{
			GPIOA->BSRR |=0x20 ; // set the PA5 pin ( LED on )	
				//GPIOA->BSRR |= GPIO_BSRR_BR5;
		}
	}
		
		
}