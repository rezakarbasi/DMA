/*
 * Support and FAQ: visit <a href="http://www.kn2c.ir">KN2C Robotic Team</a>
 */
#include <asf.h>
#include <init.h>
#include <stdio.h>

int main (void)
{
	Board_Initialize();
	
	force_boot_loader();
	
	Usart_init();
	
	udc_start();
	 
	irq_initialize_vectors();
	cpu_irq_enable();
	
	ioport_set_pin_level(LED_WHITE,1);
	ioport_set_pin_level(LED_GREEN,0);
	ioport_set_pin_level(LED_BLUE,1);
	
	source[0]='a';
	source[1]='b';
	source[2]='1';
	source[3]='2';
	
	delay_ms(1000);
	
	dma_init();
	
	//dma_channel_trigger_block_transfer(DMA_CHANNEL);
	
	//dma_channel_trigger_block_transfer(DMA_CHANNEL);
	
	while(1)
	{
		ioport_toggle_pin_level(LED_GREEN);
		
		delay_ms(1000);
		dma_channel_enable(DMA_CHANNEL);
	}
}

ISR(USARTE0_RXC_vect)
{
	ioport_toggle_pin_level(LED_BLUE);
	char a= usart_getchar(USART_SERIAL);
}