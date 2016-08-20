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
	
	spi_init();
	
	irq_initialize_vectors();
	cpu_irq_enable();
	
	ioport_set_pin_level(LED_WHITE,1);
	ioport_set_pin_level(LED_GREEN,0);
	ioport_set_pin_level(LED_BLUE,1);
	
	source[0]=3;
	source[1]=6;
	source[2]=12;
	source[3]=24;
	
	delay_ms(1000);
	
	dma_init();
	
	spi_put(&SPIC,0);
	
	while(1)
	{
		ioport_toggle_pin_level(LED_GREEN);
		
		delay_ms(50);
		
		spi_put(&SPIC,0);
		
		dma_channel_enable(DMA_CHANNEL);
		
		//udi_cdc_putc('a');
// 		int k=1;
// 		for(int i=0;i<10;i++)
// 		{
// 			delay_ms(1);
// 			spi_put(&SPIC,k);
// 			k=k*2;
// 		}
		//dma_channel_enable(DMA_CHANNEL);
	}
}

ISR(USARTE0_RXC_vect)
{
	ioport_toggle_pin_level(LED_BLUE);
	char a= usart_getchar(USART_SERIAL);
}
