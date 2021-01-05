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

#include "hdl_key.h"
#include "global_event_queue.h"

/////////////////////////////////////////////////////////////////////

void Qedc_init(void)
{
	QDEC_InputAchTypeDef channelA = PC2A;
	QDEC_InputBchTypeDef channelB = PC3B;
	QDEC_ModeTypeDef mode = DOUBLE_ACCURACY_MODE;


	gpio_set_func(GPIO_PC2, AS_GPIO);
	gpio_set_output_en(GPIO_PC2,0);
	gpio_set_input_en(GPIO_PC2,1);
	gpio_set_func(GPIO_PC3, AS_GPIO);
	gpio_set_output_en(GPIO_PC3,0);
	gpio_set_input_en(GPIO_PC3,1);

	qdec_clk_en();
	qdec_set_mode( mode);
	qdec_set_pin( channelA, channelB);
	qdec_set_debouncing(1);
	
}

void key_init(void)
{

	gpio_set_func(ONOFF_PIN, AS_GPIO);
	gpio_setup_up_down_resistor(ONOFF_PIN, PM_PIN_PULLUP_10K);
	gpio_set_output_en(ONOFF_PIN, 0);
	gpio_set_input_en(ONOFF_PIN, 1);

	gpio_set_func(RESET_PIN, AS_GPIO);
	gpio_setup_up_down_resistor(RESET_PIN, PM_PIN_PULLUP_10K);
	gpio_set_output_en(RESET_PIN, 0);
	gpio_set_input_en(RESET_PIN, 1);

	gpio_set_func(PAIR_PIN, AS_GPIO);
	gpio_setup_up_down_resistor(PAIR_PIN, PM_PIN_PULLUP_10K);
	gpio_set_output_en(PAIR_PIN, 0);
	gpio_set_input_en(PAIR_PIN, 1);

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


int qedc_value=0;


void key_scan(void)
{
	static u16 key1_press_time = 0;
	static u8 key1_run_once = 1;

	static u8 pair_in=1;
	static u8 pair_out=0;

	if (ONOFF_PIN_READ == 0)
	{
		if (key1_run_once)
			key1_press_time++;
		if (key1_press_time > 500)//5S
		{
			key1_press_time = 0;
			key1_run_once = 0;
			event_establish(EVENT_ONOFF_LONG);
		}
	
	}
	else if (!(ONOFF_PIN_READ == 0))
	{
		key1_run_once = 1;			//开启下次长按检测
		if (key1_press_time > 0 && key1_press_time < 500)
		{
			key1_press_time = 0;
			event_establish(EVENT_ONOFF_ONE);
		}
	}


	if(! (PAIR_PIN_READ == 0))//弹入
	{
		pair_in = 1;
		if (pair_out)//上次为弹出
		{
			pair_out=0;
			event_establish(EVENT_PAIR_IN);
		}
	}
	else if(PAIR_PIN_READ == 0)//弹出
	{
		pair_out=1;
		if (pair_in)//上次为弹入
		{
			pair_in=0;
			event_establish(EVENT_PAIR_OUT);
		}
	}
    qedc_value = qdec_get_count_value();
	
}


