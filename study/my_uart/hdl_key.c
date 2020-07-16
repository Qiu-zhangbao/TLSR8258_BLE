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

#if (DEVICE_TYPE == REMOTE)

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

/*


按键短按和长按任务：


1、按下按键，实现短按和长按检测
2、在按下按键的时候开始计时，在释放按键的时候结束计时，规定小于1S为短按，否则为长按
3、当按键按下时间大于1S释放时，触发一次长按事件，当按键释放时（>1S释放），不响应
4、当按键按下时间小于1S释放时，触发一次短按事件

长按事件：灯亮1S，灭1s，重复2次
短按事件：灯亮200ms,灭1s，重复2次



*/

void key_scan(void)
{
	static u16 key1_press_time = 0;
	static u16 key2_press_time = 0;
	static u8 key1_run_once = 1;		  //跑一次
	static u8 key2_run_once = 1;		  //跑一次
	static u8 key1_and_key2_run_once = 1; //跑一次

	if (KEY1 == 0)
	{
		if (!(KEY2 == 0)&&key1_run_once)
			key1_press_time++;
		if (key1_press_time > 100)
		{
			key1_press_time = 0;
			key1_run_once = 0;
			event_establish(EVENT_KEY1_LONG_PRESSED);
		}
		else if (key1_press_time > 0 && key1_press_time < 100)
		{
			if (KEY2 == 0)
			{
				key1_press_time = 0;
				key1_run_once = 0;
				key2_run_once = 0; //不检测key2短按和长按
				event_establish(EVENT_KEY1_AND_KEY2_PRESSED);
			}
		}
	}
	else if (!(KEY1 == 0))
	{
		key1_run_once = 1;			//开启下次长按检测
		key1_and_key2_run_once = 1; //开启下次双按检测
		if (key1_press_time > 0 && key1_press_time < 100)
		{
			key1_press_time = 0;
			event_establish(EVENT_KEY1_SHORT_PRESSED);
		}
	}

	if (KEY2 == 0)
	{
		if (key2_run_once)
			key2_press_time++;
		if (key2_press_time > 100)
		{
			key2_press_time = 0;
			key2_run_once = 0;
			event_establish(EVENT_KEY2_LONG_PRESSED);
		}
	}
	else if (!(KEY2 == 0))
	{
		key2_run_once = 1;			//开启下次长按检测
		key1_and_key2_run_once = 1; //开启下次双按检测
		if (key2_press_time > 0 && key2_press_time < 100)
		{
			key2_press_time = 0;
			event_establish(EVENT_KEY2_SHORT_PRESSED);
		}
	}
}

#endif
