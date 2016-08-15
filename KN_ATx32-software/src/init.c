/*
 * init.c
 *
 * Created: 8/13/2016 4:10:30 PM
 *  Author: Reza
 */ 

#include <init.h>


uint8_t source[DMA_BUFFER_SIZE];
struct Reza destination;

void force_boot_loader(void)
{
	ioport_set_pin_level(LED_WHITE,0);
	ioport_set_pin_level(LED_BLUE,0);
	ioport_set_pin_level(LED_GREEN,0);
	if ( !ioport_get_pin_level(BUTTON_1) |  !ioport_get_pin_level(BUTTON_0))
	{
		goto *(BOOT_SECTION_START + 0x1FC) ;
	}
}

inline void Board_Initialize(void)
{
		sysclk_init();
		ioport_init();
	 	
	 	ioport_set_pin_dir(LED_BLUE, IOPORT_DIR_OUTPUT);
	 	ioport_set_pin_dir(LED_GREEN, IOPORT_DIR_OUTPUT);
	 	ioport_set_pin_dir(LED_WHITE, IOPORT_DIR_OUTPUT);
	 	
	 	ioport_configure_pin(BUTTON_0, IOPORT_PULL_UP);
	 	ioport_configure_pin(BUTTON_1, IOPORT_PULL_UP);
 }

void Usart_init()
{
	static usart_rs232_options_t USART_SERIAL_OPTIONS = {
		.baudrate = USART_SERIAL_BAUDRATE,
		.charlength = USART_SERIAL_CHAR_LENGTH,
		.paritytype = USART_SERIAL_PARITY,
		.stopbits = USART_SERIAL_STOP_BIT
	};
	sysclk_enable_module(SYSCLK_PORT_E, PR_USART0_bm);
	ioport_configure_pin(RX,IOPORT_DIR_INPUT);
	ioport_configure_pin(TX,IOPORT_DIR_OUTPUT);
	usart_init_rs232(USART_SERIAL, &USART_SERIAL_OPTIONS);
	usart_set_rx_interrupt_level(USART_SERIAL,USART_INT_LVL_HI);
}

inline void dma_init(void)
{
	struct dma_channel_config dmach_conf;
	memset(&dmach_conf, 0, sizeof(dmach_conf));
	dma_channel_set_burst_length(&dmach_conf, DMA_CH_BURSTLEN_1BYTE_gc);
	dma_channel_set_transfer_count(&dmach_conf, DMA_BUFFER_SIZE);
	dma_channel_set_src_reload_mode(&dmach_conf,
	DMA_CH_SRCRELOAD_TRANSACTION_gc);
	dma_channel_set_dest_reload_mode(&dmach_conf,
	DMA_CH_DESTRELOAD_TRANSACTION_gc);
	dma_channel_set_src_dir_mode(&dmach_conf, DMA_CH_SRCDIR_INC_gc);
	dma_channel_set_source_address(&dmach_conf,
	(uint16_t)(uintptr_t)source);
	dma_channel_set_dest_dir_mode(&dmach_conf, DMA_CH_DESTDIR_INC_gc);
	dma_channel_set_destination_address(&dmach_conf,
	(uint16_t)(uintptr_t)&destination);
	dma_enable();
	dma_channel_write_config(DMA_CHANNEL, &dmach_conf);
	dma_channel_enable(DMA_CHANNEL);
};