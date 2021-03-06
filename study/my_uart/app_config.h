/********************************************************************************************************
 * @file     app_config.h 
 *
 * @brief    for TLSR chips
 *
 * @author	 public@telink-semi.com;
 * @date     May. 12, 2018
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
#pragma once

#include "vendor/common/blt_led.h"

/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif

/////////////////// Device Select /////////////////////////////////

#define REMOTE  	1

#define LIGHT 	 	2

#define DEVICE_TYPE  	LIGHT

#define KIT  		1

#define DONG 	 	2

#define DEVICE_BOARD  	KIT

#define REMOTE_SOFTWARE_VISON  	1.4
/*
遥控器1.2稳定版

*/
#define LIGHT_SOFTWARE_VISON  	1.4

/*
灯1.2存在写flash效率低问题
*/





/*
变更记录

20200929
版本1.4

1、改变灯的引脚，PWM PC4
2、灯上电6次复位
3、遥控器OLED不初始化和不输出IIC数据
4、遥控器灯的引脚更改

20200730
版本1.3

1、增加灯log打印
2、灯在无操作5s后保存数据


20200729
版本1.2


1、灯被多个遥控器绑定
2、能交叉控制，灯以收到遥控器命令的先后顺序受控
3、使用随机mac地址，设备过滤用广播的形式
4、遥控器不用一直广播命令，发完命令自动关闭广播和扫描
5、灯的开关状态以上次掉电状态为准（由于flash问题代码被注释）
6、用事件触发任务，增加响应速度



*/

/////////////////// Clock  /////////////////////////////////
#define CLOCK_SYS_CLOCK_HZ  	16000000

enum{
	CLOCK_SYS_CLOCK_1S = CLOCK_SYS_CLOCK_HZ,
	CLOCK_SYS_CLOCK_1MS = (CLOCK_SYS_CLOCK_1S / 1000),
	CLOCK_SYS_CLOCK_1US = (CLOCK_SYS_CLOCK_1S / 1000000),
};
#define BLT_APP_LED_ENABLE					1

//////////////////// LED CONFIG (EVK board) ///////////////////////////

#if (DEVICE_BOARD == KIT)
#if (DEVICE_TYPE == REMOTE)
#define	GPIO_LED						GPIO_PC1//123:B4 C4 C3:
#define LED_ON_LEVAL					0			//gpio output high voltage to turn on led

#else
#define	GPIO_LED						GPIO_PC4//123:B4 C4 C3:
#define LED_ON_LEVAL					1			//gpio output high voltage to turn on led

#endif
// #elif(DEVICE_BOARD == DONG)
// #define	GPIO_LED					//	GPIO_PC0
#endif



#if (DEVICE_TYPE == REMOTE)
enum{
	REMOTE_LED_KEY_PRESS,
	REMOTE_LED_BOUND_SUCCESS,
	REMOTE_LED_BOUND_FAIL,
	REMOTE_LED_UNBOUND_SUCCESS,
	REMOTE_LED_UNBOUND_FAIL,
	REMOTE_LED_BOUND_MODE,
	REMOTE_LED_UNBOUND_MODE,
	LIGHT_LED_RECOVER,
};
#elif(DEVICE_TYPE == LIGHT)
enum{
	LIGHT_LED_ON = 0,
	LIGHT_LED_OFF,
	LIGHT_LED_SELECT,
	LIGHT_LED_RECOVER,
	LIGHT_LED_BOUND_ACK,
	LIGHT_LED_UNBOUND_ACK,
	LIGHT_LED_QUACK_FLASH,
};
#endif

enum
{
    STORAGE_BOUND_MAC = 1,
	STORAGE_RESET_CNT,
	STORAGE_LIGHT_STATE,
};

//////////////////// DMA ///////////////////////////
#define 	SLAVE_DMA_MODE_ADDR_WRITE		0x44a00  //i2c master write data to  0x4A000
#define 	SLAVE_DMA_MODE_ADDR_READ		0x4A100  //i2c master read data from 0x4A100

extern char at_print_buf[256];
extern led_cfg_t led_cfg[];



#include "vendor/common/default_config.h"

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
