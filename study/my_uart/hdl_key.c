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

#define UI_BUTTON_ENABLE   1
#if (UI_BUTTON_ENABLE)

/////////////////////////////////////////////////////////////////////

#define BTN_VALID_LEVEL 0

#define BTN_PAIR 0x01
#define BTN_UNPAIR 0x02

u32 ctrl_btn[] = {KEY1_PIN, KEY2_PIN};
u8 btn_map[MAX_BTN_SIZE] = {BTN_PAIR, BTN_UNPAIR};


vc_data_t vc_event;
btn_status_t btn_status;
key_state_t key_state=KEY_RELASE;

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

u8 btn_debounce_filter(u8 *btn_v)
{
	u8 change = 0;

	for (int i = 3; i > 0; i--)
	{
		btn_status.btn_history[i] = btn_status.btn_history[i - 1];
	}
	btn_status.btn_history[0] = *btn_v;

	if (btn_status.btn_history[0] == btn_status.btn_history[1] && btn_status.btn_history[1] == btn_status.btn_history[2] &&
		btn_status.btn_history[0] != btn_status.btn_filter_last)
	{
		change = 1;

		btn_status.btn_filter_last = btn_status.btn_history[0];
	}

	return change;
}

u8 vc_detect_button(int read_key)
{
	u8 btn_changed, i;
	memset(&vc_event, 0, sizeof(vc_data_t)); //clear vc_event
	//vc_event.btn_press = 0;

	for (i = 0; i < MAX_BTN_SIZE; i++)
	{
		if (BTN_VALID_LEVEL != !gpio_read(ctrl_btn[i]))
		{
			vc_event.btn_press |= BIT(i);
		}
	}

	btn_changed = btn_debounce_filter(&vc_event.btn_press);

	if (btn_changed && read_key)
	{
		for (i = 0; i < MAX_BTN_SIZE; i++)
		{
			if (vc_event.btn_press & BIT(i))
			{
				vc_event.keycode[vc_event.cnt++] = btn_map[i];
			}
		}

		return 1;
	}

	return 0;
}

void proc_button(void)
{
    static u32 button_det_tick;
    if(clock_time_exceed(button_det_tick, 5000))
    {
        button_det_tick = clock_time();
    }
    else{
        return;
    }

	int det_key = vc_detect_button(1);

	if (det_key) //key change: press or release
	{
		u8 key0 = vc_event.keycode[0];
		//			u8 key1 = vc_event.keycode[1];

		if (vc_event.cnt == 2) //two key press
		{
            key_state=KEY1_KEY2_PRESS;
		}
		else if (vc_event.cnt == 1) //one key press
		{
			if (key0 == BTN_PAIR)
			{
				 key_state=KEY1_SHORT_PRESS;
			}
			else if (key0 == BTN_UNPAIR)
			{
				 key_state=KEY2_SHORT_PRESS;
			}
		}
		else
		{ 
            key_state=KEY_RELASE;
		}
	}
}

key_state_t btn_status_last=KEY_RELASE;

// key_state_t key_scan(void)
// {
//     proc_button();

//     static u32 press_time=0;
    
//     if(key_state == KEY1_SHORT_PRESS)
//     {
//         press_time++;
//         if(press_time>200)
//         {
//             press_time=0;
//             key_state = KEY1_LONG_PRESS;
//         }
//     }
//     else if(key_state == KEY2_SHORT_PRESS)
//     {
//         press_time++;
//         if(press_time>200)
//         {
//             press_time=0;
//             key_state = KEY2_LONG_PRESS;
//         }
//     }
//     else  if(key_state == KEY_RELASE)
//     {
//         if(btn_status_last!=key_state)
//         {
//             return btn_status_last;
//         }
       
//     }
    
//     btn_status_last=key_state;

// }

// key_state_t key_scan(void)
// {
//     if(KEY1 == 0)
//     {

//     }
//     else if 
//     {
//         /* code */
//     }
    
// }














#endif //end of UI_BUTTON_ENABLE
