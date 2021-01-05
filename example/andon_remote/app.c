/********************************************************************************************************
 * @file     feature_adv_power.c 
 *
 * @brief    for TLSR chips
 *
 * @author	 public@telink-semi.com;
 * @date     May. 10, 2018
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

#include "stack/ble/ble.h"
#include "tl_common.h"
#include "drivers.h"
#include "tinyFlash/tinyFlash.h"
#include "vendor/common/blt_led.h"
#include "vendor/common/blt_soft_timer.h"
#include "vendor/common/blt_common.h"

#include "global_event_queue.h"

#include "app_config.h"
#include "apc_fun_sm.h"

#include "bsl_adv.h"

#include "hdl_key.h"
#include "hdl_uart.h"


#define	MY_RF_POWER_INDEX	RF_POWER_P3p01dBm



#define RX_FIFO_SIZE						288  //rx-24   max:251+24 = 275  16 align-> 288
#define RX_FIFO_NUM							8

#define TX_FIFO_SIZE						264  //tx-12   max:251+12 = 263  4 align-> 264
#define TX_FIFO_NUM							8

MYFIFO_INIT(blt_rxfifo, RX_FIFO_SIZE, RX_FIFO_NUM);
MYFIFO_INIT(blt_txfifo, TX_FIFO_SIZE, TX_FIFO_NUM);

#define MTU_SIZE_SETTING   			 		247

/////////////////////PARA////////////////////////////
u8  device_mac_adr[6];
 //u8  bound_mac_adr[6]={0x1F,0x21,0xF2,0x38,0xC1,0xA4};
//u8  bound_mac_adr[6]={0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
u8  bound_mac_adr[6]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
u8 reset_cnt=0;

u8 bound_remote_list[10][6]={{0}};

u8 global_light_state=0;

int ui_process(void);

//////////////////////////////////////////////////////////
// event call back
//////////////////////////////////////////////////////////
int controller_event_callback (u32 h, u8 *p, int n)
{
	if (h &HCI_FLAG_EVENT_BT_STD)		//ble controller hci event
	{
		u8 evtCode = h & 0xff;
		if(evtCode == HCI_EVT_LE_META)
		{
			u8 subEvt_code = p[0];
			//--------hci le event: le adv report event ----------------------------------------
			if (subEvt_code == HCI_SUB_EVT_LE_ADVERTISING_REPORT)	// ADV packet
			{
				//after controller is set to scan state, it will report all the adv packet it received by this event
				event_adv_report_t *pa = (event_adv_report_t *)p;
				if(pa->data[10]==0x02 && pa->data[11]==0x10 && pa->data[12]==0x66)//筛选灯和遥控器
				{
					bsl_adv_recive_data(pa->data, pa->len);
				// at_print_array(pa->data, pa->len);
				// at_print("\r\n");
				}
			
			}
		}
	}
	
	return 0;
}

void adv_scan(void)
{
	blc_initMacAddress(CFG_ADR_MAC, device_mac_adr); //初始化MAC地址

	////// Controller Initialization  //////////
	blc_ll_initBasicMCU();   //初始化MCU

	blc_ll_initStandby_module(device_mac_adr);		//初始化蓝牙待机功能模块

	blc_ll_initAdvertising_module(device_mac_adr);  //初始化蓝牙广播功能模块
	blc_ll_initScanning_module(device_mac_adr); 	//scan module: 		 mandatory for BLE master,

	u8 status = bls_ll_setAdvParam( ADV_INTERVAL_10MS , //广播时间间隔最小值
									ADV_INTERVAL_10MS , //广播时间间隔最大值
									ADV_TYPE_NONCONNECTABLE_UNDIRECTED, //广播类型，不可连接非定向
									OWN_ADDRESS_PUBLIC, //自身地址类型
									0,  //定向地址类型
									NULL, //定向地址
									BLT_ENABLE_ADV_ALL, //在全部广播信道(37,38,39)都广播数据
									ADV_FP_NONE); //过滤策略


	if(status != BLE_SUCCESS)//如果设置广播参数失败
	{
		write_reg8(0x40000, 0x11);  //debug
		while(1);
	}

	blc_hci_registerControllerEventHandler(controller_event_callback); //controller hci event to host all processed in this func

	blc_hci_le_setEventMask_cmd(HCI_LE_EVT_MASK_ADVERTISING_REPORT);

	blc_ll_setScanParameter(SCAN_TYPE_PASSIVE, SCAN_INTERVAL_10MS, SCAN_INTERVAL_300MS, OWN_ADDRESS_PUBLIC, SCAN_FP_ALLOW_ADV_ANY);

	blc_ll_addScanningInAdvState();
}


void global_var_init(void)
{
	// tinyFlash_Init(0x70000,0x4000); //初始化KV存储系统
	// u8 len=60;
	// // tinyFlash_Format();
	// //tinyFlash_Write(STORAGE_BOUND_MAC, bound_mac_adr, sizeof(bound_mac_adr)); 
	// tinyFlash_Read(STORAGE_BOUND_MAC, bound_remote_list, &len); 

}

///////////////////////////////////////////////////////上电五次清数据////////////////////////////////////////////////////////////

// int reset_cnt_clera(void)
// {
// 	reset_cnt = 0;
// 	tinyFlash_Write(STORAGE_RESET_CNT, &reset_cnt, 1);
// 	return -1;
// }
// void factory_reset_cnt_check (void)
// {
// 	u8 len=1;
// 	tinyFlash_Read(STORAGE_RESET_CNT, &reset_cnt, &len); 
// 	if (reset_cnt>5)//第5次进来
// 	{
// 		reset_cnt=0;
// 		tinyFlash_Write(STORAGE_RESET_CNT, &reset_cnt, 1); //复位标记

// 		for (u8 i = 0; i < 10; i++)
// 		{
// 			for (u8 j = 0; j < 6; j++)
// 			{
// 				bound_remote_list[i][j]=0;
// 			}
// 		}
		
// 		tinyFlash_Write(STORAGE_BOUND_MAC, (unsigned char *)bound_remote_list, sizeof(bound_remote_list)); //清除地址

// 		//device_led_setup(led_cfg[LIGHT_LED_RECOVER]);//闪烁
// 	}
// 	else
// 	{
// 		reset_cnt++;
// 		tinyFlash_Write(STORAGE_RESET_CNT, &reset_cnt, 1);
// 		blt_soft_timer_add(reset_cnt_clera,8000*1000);//100ms
// 	}
// }

//////////////////////////////////////////////////////初始化/////////////////////////////////////////////////////////
void user_init_normal(void)
{
	random_generator_init();  //初始化随机数生成器
	adv_scan();
	rf_set_power_level_index (MY_RF_POWER_INDEX); //设置发射功率
	app_uart_init(); //初始化串口，用于调试打印输出
	irq_enable();
	global_var_init();
	blt_soft_timer_init();
	blt_soft_timer_add(ui_process,10*1000);//100ms
	Init_event_queue();//事件队列
	
	key_init();
	Qedc_init();
	apc_fun_sm_init();


	device_led_init(GPIO_PC0, LED_ON_LEVAL);  //LED initialization

	// //factory_reset_cnt_check();
	bsl_adv_init();

	// at_print("\r\ninit OK\r\n");
}

int ui_process(void)
{
	key_scan();

	#if (BLT_APP_LED_ENABLE)
	device_led_process();
	#endif

	return 0;
}

_attribute_ram_code_ void main_loop (void)
{
	blt_sdk_main_loop();
	blt_soft_timer_process(MAINLOOP_ENTRY);
}

