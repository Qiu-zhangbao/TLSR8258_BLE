#ifndef __KEY_H
#define __KEY_H			
  	 
#include "tl_common.h"
#include "drivers.h"


#define ONOFF_PIN    GPIO_PA1
#define RESET_PIN    GPIO_PB7
#define PAIR_PIN    GPIO_PD7


#define ONOFF_PIN_READ gpio_read(ONOFF_PIN)
#define RESET_PIN_READ gpio_read(RESET_PIN)
#define PAIR_PIN_READ gpio_read(PAIR_PIN)


void key_init(void);
void key_scan(void);
void Qedc_init(void);

#endif



