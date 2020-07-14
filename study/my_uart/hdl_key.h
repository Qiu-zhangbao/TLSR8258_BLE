#ifndef __KEY_H
#define __KEY_H			
  	 
#include "tl_common.h"
#include "drivers.h"

#define KEY1_PIN    GPIO_PD2
#define KEY2_PIN    GPIO_PB5

#define MAX_BTN_SIZE 2
typedef struct
{
	u8 cnt; //count button num
	u8 btn_press;
	u8 keycode[MAX_BTN_SIZE]; //6 btn
} vc_data_t;


typedef struct
{
	u8 btn_history[4]; //vc history btn save
	u8 btn_filter_last;
	u8 btn_not_release;
	u8 btn_new; //new btn  flag
} btn_status_t;



#define KEY1 gpio_read(KEY1_PIN)
#define KEY2 gpio_read(KEY2_PIN)

typedef enum
{
    KEY1_SHORT_PRESS=0,
    KEY2_SHORT_PRESS,
    KEY1_LONG_PRESS,
    KEY2_LONG_PRESS,
    KEY1_KEY2_PRESS,
    KEY_RELASE
}key_state_t;


void key_init(void);
void proc_button(void);
key_state_t key_scan(void);


#endif



