/********************************************************************************************************
 * @file	 button.c
 *
 * @brief    for TLSR chips
 *
 * @author	 public@telink-semi.com;
 * @date     Sep. 18, 2018
 *
 * @par      Copyright (c) Telink Semiconductor (Shanghai) Co., Ltd.
 *           All rights reserved.
 *
 *			 The information contained herein is confidential and proprietary property of Telink
 * 		     Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *			 of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *			 Co., Ltd. and the licensee in separate contract or the terms described here-in.
 *           This heading MUST NOT be removed from this file.
 *
 * 			 Licensees are granted free, non-transferable use of the information in this
 *			 file under Mutual Non-Disclosure Agreement. NO WARRENTY of ANY KIND is provided.
 *
 *******************************************************************************************************/

#include "tl_common.h"
#include "drivers.h"
#include "hdl_key.h"
#include "global_event_queue.h"
#define UI_BUTTON_ENABLE   1
#if (UI_BUTTON_ENABLE)

/////////////////////////////////////////////////////////////////////


void key_init(void)
{
    gpio_set_func(GPIO_PD2, AS_GPIO);
	gpio_setup_up_down_resistor(GPIO_PD2, PM_PIN_PULLUP_10K);
	gpio_set_output_en(GPIO_PD2, 0);
	gpio_set_input_en(GPIO_PD2, 1); 	

	gpio_set_func(GPIO_PB5, AS_GPIO);
	gpio_setup_up_down_resistor(GPIO_PB5, PM_PIN_PULLUP_10K);
	gpio_set_output_en(GPIO_PB5, 0);
	gpio_set_input_en(GPIO_PB5, 1); 
}


void key_scan(void)
{
	static u16 time = 0;
	static u8 flag1=0;
	static u8 flag2=0;
	static u8 flag1_long=0;
	static u8 flag2_long=0;

    if(KEY1 == 0)
    {
		time++;
		flag1 = 1;
		if(time>100&&flag1_long==0)
		{
			time=0;
			flag1 = 0;
			flag1_long = 1;
			event_establish(EVENT_KEY1_LONG_PRESSED);
		}
		
    }
    else if (!(KEY1 == 0)&&flag1 == 1)
    {
		time=0;
		flag1 = 0;
		flag1_long=0;
		event_establish(EVENT_KEY1_SHORT_PRESSED);
    }


	if(KEY2 == 0)
    {
		time++;
		flag2 = 1;
		if(time>100&&flag2_long==0)
		{
			time=0;
			flag2 = 0;
			flag2_long = 1;
			event_establish(EVENT_KEY2_LONG_PRESSED);
		}
		
    }
    else if (!(KEY2 == 0)&&flag2 == 1)
    {
		time=0;
		flag2 = 0;
		flag2_long=0;
		event_establish(EVENT_KEY2_SHORT_PRESSED);
    }

}














#endif //end of UI_BUTTON_ENABLE
