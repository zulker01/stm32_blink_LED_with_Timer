#include "stm32f4xx.h"

#define GPIOAEN   (1U<<0)  //enable gpioa
#define UART2EN   ( 1U<<17)
#define SYS_FREQ 16000000 // 16 MHz
#define APB1_CLK SYS_FREQ
#define CR1_TE (1U<<3)  // usart transmission enabled

#define CR1_UE (1U<<3)   // usart power on
#define UART_BAUDRATE 115200

static void uart_set_baudrate(USART_TypeDef *UARTx, uint32_t PeriphClk, uint32_t BaudRate);
static uint16_t compute_uart_bd(uint32_t PeriphClk,uint32_t BaudRate);


int main(void)
{
	while(1)
	{
	}
	
}

void uart2_tx_init(void)
{
	// 1. configure uart gpio pin 
		// enable clock access to gpio
	RCC->AHB1ENR |=GPIOAEN;
	
	// set pa2 pin to alternate func mode
	 GPIOA->MODER &=~(1U<<4);
	GPIOA->MODER |= (1U<<5);
	
	// pa2 af type to uart_tx(af07) set bits 8-9-10-11 = 0111
	// afr[0] is for afrl low, afr[1] fpr afrh 
	GPIOA->AFR[0] |=(1U<<8); // set 1
GPIOA->AFR[0] |=(1U<<9); //set 1
GPIOA->AFR[0] |=(1U<<10); // set 1 
GPIOA->AFR[0] &=~(1U<<11); // set zero

// configu uart module

// enable clock of uart2
RCC->APB1ENR |=UART2EN;  

// cinfug baudrate
uart_set_baudrate(USART2,APB1_CLK,UART_BAUDRATE);

// config the transfer direction
USART2->CR1 = CR1_TE;   // just 3rd bit is set, others are zero for default
// enable uart module 

USART2->CR1 |= CR1_UE;   
}

static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t BaudRate)
{
	USARTx->BRR  = compute_uart_bd(PeriphClk,BaudRate	);
}

static uint16_t compute_uart_bd(uint32_t PeriphClk,uint32_t BaudRate)
{
	return (PeriphClk +(BaudRate/2U)/BaudRate);
}