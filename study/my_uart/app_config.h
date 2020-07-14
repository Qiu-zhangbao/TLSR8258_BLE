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

/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif

/////////////////// Clock  /////////////////////////////////
#define CLOCK_SYS_CLOCK_HZ  	16000000

enum{
	CLOCK_SYS_CLOCK_1S = CLOCK_SYS_CLOCK_HZ,
	CLOCK_SYS_CLOCK_1MS = (CLOCK_SYS_CLOCK_1S / 1000),
	CLOCK_SYS_CLOCK_1US = (CLOCK_SYS_CLOCK_1S / 1000000),
};
#define BLT_APP_LED_ENABLE					1

//////////////////// LED CONFIG (EVK board) ///////////////////////////
// #if (BLT_APP_LED_ENABLE)
	// #define LED_ON_LEVAL 					1 			//gpio output high voltage to turn on led
	// #define	GPIO_LED						GPIO_PB4    //red
	// #define PB4_FUNC						AS_GPIO
// #endif


#define LED_ON_LEVAL					1			//gpio output high voltage to turn on led
#define	GPIO_LED						GPIO_PC4    //red


#define 	SLAVE_DMA_MODE_ADDR_WRITE		0x44a00  //i2c master write data to  0x4A000
#define 	SLAVE_DMA_MODE_ADDR_READ		0x4A100  //i2c master read data from 0x4A100

extern char at_print_buf[256];


#include "vendor/common/default_config.h"

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
