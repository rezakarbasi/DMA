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
	
	
	for (char i=0;i<DMA_BUFFER_SIZE;i++)
	{
		source[i]=i;
	}

	destination.a=1;
	destination.b=2;
	destination.c=3;

	
	delay_ms(1000);
	
	dma_init();
	
	char str[50];
	int counter=sprintf(str,"First : \r.a = %d\r.b = %d\r.c = %d",destination.a,destination.b,destination.c);
	for(char i=0;i<counter;i++)usart_putchar(USART_SERIAL,str[i]);
	
	dma_channel_trigger_block_transfer(DMA_CHANNEL);

	delay_ms(5);
	
	counter=sprintf(str,"\r\r\r\r\rSecond : \r.a = %d\r.b = %d\r.c = %d",destination.a,destination.b,destination.c);
	for(char i=0;i<counter;i++)usart_putchar(USART_SERIAL,str[i]);
	
	while(1)
	{
		ioport_toggle_pin_level(LED_WHITE);
		
		delay_ms(1000);
	}
}

ISR(USARTE0_RXC_vect)
{
	ioport_toggle_pin_level(LED_BLUE);
	char a= usart_getchar(USART_SERIAL);
}