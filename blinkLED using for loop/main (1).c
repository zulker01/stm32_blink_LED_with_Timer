#include "stm32f446xx.h"

int main(){
		RCC->AHB1ENR |= 0x1;// or with 
		GPIOA->MODER |= 0x400;
		while(1){
				GPIOA->BSRR |= GPIO_BSRR_BS5;
				for(int i=0; i<1000000; i++) {}
					GPIOA->BSRR |= GPIO_BSRR_BR5;
					
					for(int i=0; i<1000000; i++)
{
}	
				
		}
}
