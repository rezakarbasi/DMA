/*
 * init.h
 *
 * Created: 8/13/2016 4:09:35 PM
 *  Author: Reza
 */ 


#ifndef INIT_H_
#define INIT_H_

#include <conf_board.h>
#include <asf.h>

#define USART_SERIAL                     &USARTE0
#define USART_SERIAL_BAUDRATE            9600
#define USART_SERIAL_CHAR_LENGTH         USART_CHSIZE_8BIT_gc
#define USART_SERIAL_PARITY              USART_PMODE_DISABLED_gc
#define USART_SERIAL_STOP_BIT            false

#define DMA_CHANNEL     0
#define DMA_BUFFER_SIZE 4

struct Reza
{
	char a;
	int b;
	char c;
};

uint8_t source[DMA_BUFFER_SIZE];
struct Reza destination;

void force_boot_loader(void);

void Board_Initialize(void);

void Usart_init();

void dma_init(void);


#endif /* INIT_H_ */