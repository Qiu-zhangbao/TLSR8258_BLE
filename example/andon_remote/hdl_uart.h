
#ifndef __UART__
#define __UART__

#include "tl_common.h"

#define UART_DATA_LEN    12      //data max ?    (UART_DATA_LEN+4) must 16 byte aligned

typedef struct{
	unsigned int dma_len;        // dma len must be 4 byte
	char data[UART_DATA_LEN];
}uart_data_t;



void app_uart_init(void);
void at_print(char * str);
void at_send(char * data, u32 len);
void app_uart_irq_proc(void);
void at_print_array(char * data, u32 len);


#endif



