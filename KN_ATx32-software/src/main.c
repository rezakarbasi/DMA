/*
 * Support and FAQ: visit <a href="http://www.kn2c.ir">KN2C Robotic Team</a>
 */
#include <asf.h>
#include <init.h>
#include <stdio.h>
//#include <dma_driver.h>


#define DMA_TX_Channel &DMA.CH0
#define DMA_RX_Channel &DMA.CH1


#define TEST_CHARS  20

static char Tx_Buf[TEST_CHARS];
static char Rx_Buf[TEST_CHARS];

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
	
	delay_ms(1000);
	
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

void SetupTransmitChannel( void )
{
	DMA_SetupBlock(
	DMA_TX_Channel,
	Tx_Buf,
	DMA_CH_SRCRELOAD_NONE_gc,
	DMA_CH_SRCDIR_INC_gc,
	(void *) &USART.DATA,
	DMA_CH_DESTRELOAD_NONE_gc,
	DMA_CH_DESTDIR_FIXED_gc,
	TEST_CHARS,
	DMA_CH_BURSTLEN_1BYTE_gc,
	0, // Perform once
	false
	);
	DMA_EnableSingleShot( DMA_TX_Channel);
	// USART Trigger source, Data Register Empty, 0x4C
	dma_channel_set_trigger_source(DMA_TX_Channel,DMA_CH_TRIGSRC_USARTE0_DRE_gc);
}

void SetupReceiveChannel( void )
{
	DMA_SetupBlock(
	DMA_RX_Channel,
	(void *) &USART.DATA,
	DMA_CH_SRCRELOAD_NONE_gc,
	DMA_CH_SRCDIR_FIXED_gc,
	Rx_Buf,
	DMA_CH_DESTRELOAD_NONE_gc,
	DMA_CH_DESTDIR_INC_gc,
	TEST_CHARS,
	DMA_CH_BURSTLEN_1BYTE_gc,
	0, // Perform once
	false
	);
	
	DMA_EnableSingleShot( DMA_RX_Channel );
	// USART Trigger source, Receive complete
	dma_channel_set_trigger_source(DMA_RX_Channel,DMA_CH_TRIGSRC_USARTE0_RXC_gc);
}