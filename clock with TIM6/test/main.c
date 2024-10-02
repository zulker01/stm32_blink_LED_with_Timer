#include "stm32f446xx.h"
#include "stdio.h"

void updateMiliSec();
void updateMin();
void updateSec();
void updateHour();

int testLED=0;

int hour = 2;
int minute = 12;
int sec = 10;
int milsec = 0;
uint32_t debugCount_TIM7_IRQHandler = 0;
void updateMiliSec()
{
	milsec+=100;
	if(milsec == 1000)
	{
		milsec = 0;
		updateSec();
	}
	return ;
}
void updateSec()
{
	sec++;
	if(sec == 60 )
	{
		sec = 0;
		updateMin();
	}
	return ;
}

void updateMin()
{
	minute++;
	if(minute == 60 )
	{
		minute = 0;
		updateHour();
	}
	return ;
}

void updateHour()
{
	hour++;
	if(hour == 24 )
	{
		hour = 0;
		
	}
	return ;
}




void TIM7_IRQHandler(void){
	
	// if one pulse mood is used in main(), then timer stops at one count, no interrupt happens, LED does not on,
	// but if used in here( handler ) then LED is set on ,no blinking 
	// TIM7->CR1 |= 0x6; // one pulse mood, counter off after one count
	debugCount_TIM7_IRQHandler++;
	//if(debugCount_TIM7_IRQHandler == 60 )
		updateMiliSec();
	//GPIOA->ODR ^= 0x20;
	if(!testLED)
	{
			testLED = 1;
			GPIOA->BSRR |= GPIO_BSRR_BS5; // set PA 5
	}
	else
	{
			testLED = 0;
			GPIOA->BSRR |= GPIO_BSRR_BR5; // reset PA 5
	}
	//	TIM7->CR1&=0x0; //disable counter
	TIM7->SR &=0x0;
	//TIM7->SR &= ~TIM_SR_UIF; //c reset update flag ( used - instead of ~ )
	TIM7->CNT = 0x0; //c reset counting
}
void config_GPIO(){
	//GPIO clock enable and set PA5 in output mode
	RCC->AHB1ENR |= 0x1;
	GPIOA->MODER |= 0x400;
}
int count_Timer6_interrupt = 0;

void TIM6_DAC_IRQHandler(void){
	
	
	count_Timer6_interrupt++;
//	GPIOA->ODR ^= 0x20; //toggle LED
	if(!testLED)
	{
			testLED = 1;
			GPIOA->BSRR |= GPIO_BSRR_BS5; // set PA 5
	}
	else
	{
			testLED = 0;
			GPIOA->BSRR |= GPIO_BSRR_BR5; // reset PA 5
	}
	TIM6->SR &= ~TIM_SR_UIF;
	TIM6->CNT = 0;
}
void Config_BasicTimer6(){
	//Basic Timer 6 configuration
	RCC->APB1ENR |= 0x10; //clock enable
	TIM6->PSC = 1600 - 1; //slow down the clock >>>> when board is running in 16Mhz, Timer clock speed = 1 * BUS speed = 1*16Mhz = 16Mhz >>>> TIM6->PSC = 16000000/1600 = 10000 or 10khz 
	TIM6->ARR = 1000 - 1; //timer will count upto the value of ARR and generate a update event, TIM_ARR value = Timer speed after prescaled / Desire Freq = 10000/ 20 = 500
//	1 sec or 1000ms e count kore 10,000
//	1 ms e count kore 10
//	100 ms e count kore 1000
	//TIM6->EGR |= 0x1; // UG bit, Re-initializes the timer counter and generates an update of the registers
	TIM6->CNT = 0;
  TIM6->DIER |= 0x1; //update interrupt enable
	NVIC_EnableIRQ(TIM6_DAC_IRQn); //initialize NVIC interrupt function
	TIM6->CR1 |= 0x4; //only counter overflow or underflow generates an update interrupt
	//TIM6->CR1 |= 0x8; // Counter stops counting at the next update event
	//TIM6->CR1 |= 0x80;
	TIM6->CR1 |= 0x1; //enable counter
}
 int main(){
//		config_GPIO();
//	  Config_BasicTimer6();
	 
		// activate GPIOA bus
		RCC->AHB1ENR |= 0x1;//c or with
		// activate TIM7 bus
		RCC->APB1ENR |=0x20;
	
		GPIOA->MODER |= 0x400; //c set the pin PA 5 as output mode
		
		TIM7->PSC = 1600-1 ; // c  dec 1600,clkspeed/psc,clkspeed = 16MHz, so 16*10^6/1600 = 10^4;
		int preScalar = TIM7->PSC;
		TIM7->ARR = 1000-1 ; //c hex of 10,000,counts upto arr
		//	1 sec or 1000ms e count kore 10,000
//	1 ms e count kore 10
//	100 ms e count kore 1000
	 int testTimer = 0,timerCounter=0;
		
	  TIM7->CNT = 0x0; //c set ount at 0 ( changed hex to dec zero
		//if(TIM7->SR &1==1) then light up led
		//TIM7->SR &=0x0; //timer status resgister
		TIM7->DIER |=0x1; //c update interrupt enable
		//TIM7->EGR |=0x1; // re initialize counter,generates an UE when setted to 1
		//udis bit must be 0
		NVIC_EnableIRQ(TIM7_IRQn);  // c interrupt handler 
		
//		TIM7->CR1 |= 0x6; // one pulse mood, counter off after one count
	  TIM7->CR1 |= 0x4; //c URS bit only counter overflow or underflow generates an update
		
		//TIM7->CR1 &=0xFD ; // set the udis bit 0,UEV on
		
		TIM7->CR1 |= 0x1;     //c counter enable
		while(1){
	 }
	}
		/*while(1){
				testTimer = 0;
				testTimer = TIM7->SR ;
				testTimer &=0x1; // get the update flag
				timerCounter = TIM7->CNT;
				//testLED  = GPIOA->BSRR; // check if led it set to 1
				//testLED &= 0x20;
				printf("preScalar %d , timer %d , counter %d, led %d\n",preScalar,testTimer,timerCounter,testLED);
				if(testTimer){
					if(!testLED)
					{
						testLED = 1;
						GPIOA->BSRR |= GPIO_BSRR_BS5; // set PA 5
					}
					else
					{
						testLED = 0;
						GPIOA->BSRR |= GPIO_BSRR_BR5; // reset PA 5
					}
					
					TIM7->SR =0x0000; // reset the update flag
				}
				//for(int i=0; i<1000000; i++) {}
					
					
					//for(int i=0; i<1000000; i++)

				
		}*/

 /*
while(1){
				testTimer = 0;
				testTimer = TIM7->SR ;
				testTimer &=0x1; // get the update flag
				timerCounter = TIM7->CNT;
				//testLED  = GPIOA->BSRR; // check if led it set to 1
				//testLED &= 0x20;
				printf("preScalar %d , timer %d , counter %d, led %d\n",preScalar,testTimer,timerCounter,testLED);
				if(testTimer){
					if(!testLED)
					{
						testLED = 1;
						GPIOA->BSRR |= GPIO_BSRR_BS5; // set PA 5
					}
					else
					{
						testLED = 0;
						GPIOA->BSRR |= GPIO_BSRR_BR5; // reset PA 5
					}
					
					TIM7->SR =0x0000; // reset the update flag
				}
				//for(int i=0; i<1000000; i++) {}
					
					
					//for(int i=0; i<1000000; i++)

				
		}*/
/*
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
*/
