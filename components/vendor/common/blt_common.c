/********************************************************************************************************
 * @file     blt_common.c
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

void blc_initMacAddress(int flash_addr, u8 *mac_public)
{
	if (flash_addr == 0)
	{
		return;
	}

	u8 mac_read[8];
	flash_read_page(flash_addr, 8, mac_read);

	u8 ff_six_byte[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
	if (memcmp(mac_read, ff_six_byte, 6))
	{
		memcpy(mac_public, mac_read, 6); //copy public address from flash
	}
	else
	{ //no public address on flash

		u8 value_rand[5];
		generateRandomNum(5, value_rand);
		mac_public[0] = value_rand[0];
		mac_public[1] = value_rand[1];
		mac_public[2] = value_rand[2];

		mac_public[3] = 0x38;
		mac_public[4] = 0xC1;
		mac_public[5] = 0xA4;

		flash_write_page(flash_addr, 6, mac_public);
	}
}
