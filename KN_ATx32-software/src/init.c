/*
 * init.c
 *
 * Created: 8/13/2016 4:10:30 PM
 *  Author: Reza
 */ 

#include <init.h>


//uint8_t source[DMA_BUFFER_SIZE];
uint8_t source[DMA_BUFFER_SIZE];

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
	usart_set_rx_interrupt_level(USART_SERIAL,USART_INT_LVL_OFF);
}

inline void dma_init(void) // this project : ferestadane spi ok she
{
	struct dma_channel_config dmach_conf;
	memset(&dmach_conf, 0, sizeof(dmach_conf));
	dma_channel_set_burst_length(&dmach_conf, DMA_CH_BURSTLEN_1BYTE_gc);
	dma_channel_set_transfer_count(&dmach_conf, DMA_BUFFER_SIZE);
	dma_channel_set_src_reload_mode(&dmach_conf,
	DMA_CH_SRCRELOAD_TRANSACTION_gc);														//changed transcation bood
	dma_channel_set_dest_reload_mode(&dmach_conf,
	DMA_CH_DESTRELOAD_NONE_gc);														//changed  Atmel says : DMA_CH_DESTRELOAD_NONE_gc
	dma_channel_set_src_dir_mode(&dmach_conf, DMA_CH_SRCDIR_INC_gc);
	dma_channel_set_source_address(&dmach_conf,
	(uint16_t)(uintptr_t)source);
	dma_channel_set_dest_dir_mode(&dmach_conf, DMA_CH_DESTDIR_FIXED_gc);				//changed
	dma_channel_set_destination_address(&dmach_conf,
	(uint16_t)(uintptr_t)&(SPIC.DATA));
	
	//change
	//No Repeat
	dmach_conf.ctrla &= ~DMA_CH_REPEAT_bm;
	dmach_conf.repcnt = 0;
	//singleshot
	dmach_conf.ctrla |= DMA_CH_SINGLE_bm;
	//trigger source
	dmach_conf.trigsrc = DMA_CH_TRIGSRC_SPIC_gc;	
	//interrupt
	//dma_channel_set_interrupt_level(&dmach_conf,DMA_INT_LVL_HI);
	//dma_set_callback(DMA_CHANNEL,DMA_Interrupt);
	
	dma_enable();
	dma_channel_write_config(DMA_CHANNEL, &dmach_conf);
	dma_channel_enable(DMA_CHANNEL);
};

void DMA_Interrupt()
{
	udi_cdc_putc(source[2]);
	ioport_toggle_pin(LED_WHITE);
	dma_channel_enable(DMA_CHANNEL);
}

void DMA_SetupBlock( volatile DMA_CH_t * channel,
const void * srcAddr,
DMA_CH_SRCRELOAD_t srcReload,
DMA_CH_SRCDIR_t srcDirection,
void * destAddr,
DMA_CH_DESTRELOAD_t destReload,
DMA_CH_DESTDIR_t destDirection,
uint16_t blockSize,
DMA_CH_BURSTLEN_t burstMode,
uint8_t repeatCount,
bool useRepeat )
{
	channel->SRCADDR0 = (( (uint16_t) srcAddr) >> 0*8 ) & 0xFF;
	channel->SRCADDR1 = (( (uint16_t) srcAddr) >> 1*8 ) & 0xFF;
	channel->SRCADDR2 = 0;

	channel->DESTADDR0 = (( (uint16_t) destAddr) >> 0*8 ) & 0xFF;
	channel->DESTADDR1 = (( (uint16_t) destAddr) >> 1*8 ) & 0xFF;
	channel->DESTADDR2 = 0;

	channel->ADDRCTRL = (uint8_t) srcReload | srcDirection |
	destReload | destDirection;
	channel->TRFCNT = blockSize;
	channel->CTRLA = ( channel->CTRLA & ~( DMA_CH_BURSTLEN_gm | DMA_CH_REPEAT_bm ) ) |
	burstMode | ( useRepeat ? DMA_CH_REPEAT_bm : 0);

	if ( useRepeat ) {
		channel->REPCNT = repeatCount;
	}
}

void DMA_EnableSingleShot( volatile DMA_CH_t * channel )
{
	channel->CTRLA |= DMA_CH_SINGLE_bm;
}

void spi_init (void)
{
	ioport_set_pin_dir(IOPORT_CREATE_PIN(PORTC,4), IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(IOPORT_CREATE_PIN(PORTC,5), IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(IOPORT_CREATE_PIN(PORTC,6), IOPORT_DIR_INPUT);
	ioport_set_pin_dir(IOPORT_CREATE_PIN(PORTC,7), IOPORT_DIR_OUTPUT);
	
	sysclk_enable_peripheral_clock(&SPIC);
	spi_xmega_set_baud_div(&SPIC,8000000UL,F_CPU);
	spi_enable_master_mode(&SPIC);
	spi_enable(&SPIC);
}
