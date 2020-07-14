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
#include <stack/ble/ble.h>
#include "tl_common.h"
#include "drivers.h"
#include "app_config.h"
#include "vendor/common/blt_led.h"
#include "vendor/common/blt_soft_timer.h"
#include "vendor/common/blt_common.h"
#include "hdl_oled.h"
#include "hdl_key.h"
#include "app_uart.h"
#include "global_event_queue.h"

#define	MY_RF_POWER_INDEX	RF_POWER_P3p01dBm


/////////////////////////// led management /////////////////////
#if (BLT_APP_LED_ENABLE)

	enum{
		LED_ON = 0,
		LED_OFF,
		LED_BOUND,
		LED_UNBOUND,
		LED_CONTROL,
	};

	const led_cfg_t led_cfg[] = {
			{100,     0,      0xff,   0x00,	 },
			{0,	  	 100 ,	  0xff,	  0x02,  },
			{100,	  100 ,   3,	  0x02,  },
			{1000,	  1000 ,  3,	  0x04,	 },
			{50,	  0 ,   1,	  0x04,  },
	};

#endif

static u32 host_update_conn_param_req;

static u32 connect_event_occurTick;

static u32 dle_started_flg;
static u32 mtuExchange_started_flg;


_attribute_data_retention_ u32 cur_conn_device_hdl; //conn_handle

#define RX_FIFO_SIZE						288  //rx-24   max:251+24 = 275  16 align-> 288
#define RX_FIFO_NUM							8

#define TX_FIFO_SIZE						264  //tx-12   max:251+12 = 263  4 align-> 264
#define TX_FIFO_NUM							8

MYFIFO_INIT(blt_rxfifo, RX_FIFO_SIZE, RX_FIFO_NUM);
MYFIFO_INIT(blt_txfifo, TX_FIFO_SIZE, TX_FIFO_NUM);

#define MTU_SIZE_SETTING   			 		247

//////////////////////////////////////////////////////////
// event call back
//////////////////////////////////////////////////////////
int controller_event_callback (u32 h, u8 *p, int n)
{
	if (h &HCI_FLAG_EVENT_BT_STD)		//ble controller hci event
	{
		u8 evtCode = h & 0xff;

		//------------ disconnect -------------------------------------
		if(evtCode == HCI_EVT_DISCONNECTION_COMPLETE)  //connection terminate
		{
			event_disconnection_t	*pd = (event_disconnection_t *)p;

			//terminate reason//connection timeout
			if(pd->reason == HCI_ERR_CONN_TIMEOUT){
			}
			//peer device(slave) send terminate cmd on link layer
			else if(pd->reason == HCI_ERR_REMOTE_USER_TERM_CONN){
			}
			//master host disconnect( blm_ll_disconnect(current_connHandle, HCI_ERR_REMOTE_USER_TERM_CONN) )
			else if(pd->reason == HCI_ERR_CONN_TERM_BY_LOCAL_HOST){
			}
			 //master create connection, send conn_req, but did not received acked packet in 6 connection event
			else if(pd->reason == HCI_ERR_CONN_FAILED_TO_ESTABLISH){ //send connection establish event to host(telink defined event)
			}
			else{
			}

			u_sprintf(at_print_buf,"+DISCONNECT(%x)\r\n", pd->reason);
			at_print(at_print_buf);
			
			connect_event_occurTick = 0;
			host_update_conn_param_req = 0; //when disconnect, clear update conn flag
			cur_conn_device_hdl = 0;  //when disconnect, clear conn handle

			//MTU size exchange and data length exchange procedure must be executed on every new connection,
			//so when connection terminate, relative flags must be cleared
			dle_started_flg = 0;
			mtuExchange_started_flg = 0;

			//MTU size reset to default 23 bytes when connection terminated
			blt_att_resetEffectiveMtuSize(pd->handle | (pd->hh<<8));  //stack API, user can not change
		}
		else if(evtCode == HCI_EVT_LE_META)
		{
			u8 subEvt_code = p[0];

			//------hci le event: le connection establish event---------------------------------
			if(subEvt_code == HCI_SUB_EVT_LE_CONNECTION_ESTABLISH)  //connection establish(telink private event)
			{
				event_connection_complete_t *pCon = (event_connection_complete_t *)p;

				if (pCon->status == BLE_SUCCESS)	// status OK
				{
					at_print("OK\r\n");
					cur_conn_device_hdl = pCon->handle;   //mark conn handle, in fact this equals to BLM_CONN_HANDLE
					connect_event_occurTick = clock_time()|1;
				}
			}
			//--------hci le event: le adv report event ----------------------------------------
			else if (subEvt_code == HCI_SUB_EVT_LE_ADVERTISING_REPORT)	// ADV packet
			{
				//after controller is set to scan state, it will report all the adv packet it received by this event
				event_adv_report_t *pa = (event_adv_report_t *)p;
				s8 rssi = pa->data[pa->len];

				if(rssi!=0)
				{
				u_sprintf((char*)at_print_buf, "%d,%02X", rssi,pa->mac[5]);
				//u_sprintf((char*)at_print_buf, "+ADV:%d,%02X%02X%02X%02X%02X%02X,", rssi,pa->mac[5],pa->mac[4],pa->mac[3],pa->mac[2],pa->mac[1],pa->mac[0]);
				at_print(at_print_buf);

				//at_print_array(pa->data, pa->len);
				
				at_print("\r\n");
				
				}
			}
			//--------hci le event: le data length change event ----------------------------------------
			else if (subEvt_code == HCI_SUB_EVT_LE_DATA_LENGTH_CHANGE)
			{
				// hci_le_dataLengthChangeEvt_t* dle_param = (hci_le_dataLengthChangeEvt_t*)p;
				// printf("----- DLE exchange: -----\n");
				// printf("Effective Max Rx Octets: %d\n", dle_param->maxRxOct);
				// printf("Effective Max Tx Octets: %d\n", dle_param->maxTxOct);

				dle_started_flg = 1;
			}	
		}
	}
	
	return 0;
}

void adv_scan(void)
{
	u8  mac_public[6];
	u8  mac_random_static[6];
	blc_initMacAddress(CFG_ADR_MAC, mac_public, mac_random_static); //初始化MAC地址

	////// Controller Initialization  //////////
	blc_ll_initBasicMCU();   //初始化MCU

	blc_ll_initStandby_module(mac_public);		//初始化蓝牙待机功能模块

	blc_ll_initAdvertising_module(mac_public);  //初始化蓝牙广播功能模块
	blc_ll_initScanning_module(mac_public); 	//scan module: 		 mandatory for BLE master,

	//blc_ll_initScanning_module(mac_public); 	//scan module: 		 mandatory for BLE master,
	u8 tbl_advData[] = { 0x05, 0x09, 'A', 'B', 'C', 'D'}; //要广播的数据
	bls_ll_setAdvData( (u8 *)tbl_advData, sizeof(tbl_advData) ); //设置广播数据

	u8 status = bls_ll_setAdvParam( ADV_INTERVAL_100MS , //广播时间间隔最小值
									ADV_INTERVAL_100MS , //广播时间间隔最大值
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
	
	bls_ll_setAdvEnable(1); 

	blc_ll_addScanningInAdvState();
}
u32 t=0;
int led(void)
{
	device_led_setup(led_cfg[LED_BOUND]);
	t++;
	return 0;
}

void user_init_normal(void)
{
	random_generator_init();  //初始化随机数生成器

	adv_scan();

	rf_set_power_level_index (MY_RF_POWER_INDEX); //设置发射功率

	app_uart_init(); //初始化串口，用于调试打印输出

	irq_enable();
	OLED_Init();
	OLED_Clear();

	#if (BLT_APP_LED_ENABLE)
	device_led_init(GPIO_LED, LED_ON_LEVAL);  //LED initialization
	#endif
	blt_soft_timer_init();
	key_init();

	blt_soft_timer_add(led,1000*1000);
	Init_event_queue();//事件队列
}
u32 time_starts=0;

u8 flag=0;
void exange(void)
{
	blc_ll_setScanEnable (BLC_SCAN_DISABLE, DUP_FILTER_DISABLE);
	bls_ll_setAdvEnable(1); 
	flag=1;
}



_attribute_ram_code_ void main_loop (void)
{
	blt_sdk_main_loop();
	OLED_ShowNum(0,0,t,8,16);
	//change();
	static u32 button_detect_tick = 0;
	
	if(clock_time_exceed(button_detect_tick, 1000*1000))
	{
		// t++;
		button_detect_tick = clock_time();
		// u_sprintf((char*)at_print_buf, "num:%d", t);
		// at_print(at_print_buf);
		// at_print("\r\n");
		//exange();
	}
#if (BLT_APP_LED_ENABLE)
	device_led_process();
#endif
	blt_soft_timer_process(MAINLOOP_ENTRY);

	// if(key_scan() == KEY1_SHORT_PRESS)
	// {
	// 	device_led_setup(led_cfg[LED_BOUND]);
	// 	t=1;
	// }
	// else if(key_scan() == KEY2_SHORT_PRESS)
	// {
	// 	device_led_setup(led_cfg[LED_UNBOUND]);
	// 	t=2;
	// }
	// else if(key_scan() == KEY1_LONG_PRESS)
	// {
	// 	device_led_setup(led_cfg[LED_UNBOUND]);
	// 	t=3;
	// }
	// else if(key_scan() == KEY2_LONG_PRESS)
	// {
	// 	device_led_setup(led_cfg[LED_UNBOUND]);
	// 	t=4;
	// }
	// else if(key_scan() == KEY_RELASE)
	// {
	// 	t=0;
	// }
	
	

}