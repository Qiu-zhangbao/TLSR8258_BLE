#ifndef __KEY_H
#define __KEY_H			
  	 
#include "tl_common.h"
#include "drivers.h"

#if (DEVICE_TYPE == REMOTE)

#define KEY1_PIN    GPIO_PD2
#define KEY2_PIN    GPIO_PB5


#define KEY1 gpio_read(KEY1_PIN)
#define KEY2 gpio_read(KEY2_PIN)


void key_init(void);
void key_scan(void);

#endif
#endif



