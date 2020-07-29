#include "bsl_adv.h"
#include "fun_control.h"
#include "tl_common.h"
#include "drivers.h"
#include "global_event_queue.h"
#include "vendor/common/blt_soft_timer.h"
#include "vendor/common/blt_led.h"
#include <stack/ble/ble.h>
#include "tinyFlash/tinyFlash.h"

adv_packet_t adv_packet;
adv_date_t adv_date;
scan_date_t scan_date;

u8 all_device_adr[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

int bsl_adv_process(void);

#if (DEVICE_TYPE == REMOTE)

void (*bsl_add)(u8 (*mac)[6], u8 len);
void (*bsl_delate)(u8 (*mac)[6],u8 len);

u8 device_adv_name[5] = {'R', 'E', 'M', 'O', 'T'};

void bsl_adv_init(void)
{
	adv_packet.adv_flag_type = GAP_ADTYPE_FLAGS;
	adv_packet.adv_flag = 0x06;
	adv_packet.adv_flag_len = 2;

	adv_packet.name_type = GAP_ADTYPE_LOCAL_NAME_COMPLETE;
	memcpy(adv_packet.name, device_adv_name, sizeof(adv_packet.name));
	adv_packet.name_len = sizeof(adv_packet.name) + 1;

	adv_packet.device_type = GAP_ADTYPE_DEVICE_ID;
	adv_packet.device_id = 0x66;
	adv_packet.device_len = 2;

	adv_packet.date_type = GAP_ADTYPE_MANUFACTURER_SPECIFIC;

	memcpy(&adv_date.src_mac_adr, device_mac_adr, sizeof(adv_date.src_mac_adr)); //数据；源地址

	///////////////////change data start///////////////////
	memcpy(&adv_date.dst_mac_adr, all_device_adr, sizeof(adv_date.dst_mac_adr)); //数据：目标地址
	adv_date.op_code = 0x80;													 //开关灯命令
	adv_date.op_code_sub = 0;			
	adv_date.led_state = 0xff;
	adv_date.bound_state = 0xff;

	///////////////////change data end///////////////////

	memcpy(&adv_packet.date, &adv_date, sizeof(adv_packet.date));

	adv_packet.date_len = sizeof(adv_packet.date) + 1;

	bls_ll_setAdvData((u8 *)&adv_packet, sizeof(adv_packet)); //设置广播数据
	blt_soft_timer_add(bsl_adv_process, 10 * 1000);			  //100ms
}


void bsl_adv_add_callback(void (*add)(u8 (*mac)[6],u8 len))
{
	bsl_add = add;
}
void bsl_adv_delate_callback(void (*delate)(u8 (*mac)[6],u8 len))
{
	bsl_delate = delate;
}

void bsl_adv_led_onoff(u8 on)
{
	if (on)
	{
		///////////////////change data start///////////////////
		memcpy(&adv_date.dst_mac_adr, all_device_adr, sizeof(adv_date.dst_mac_adr)); //数据：目标地址
		adv_date.op_code = 0x80;													 //开关灯命令
		adv_date.op_code_sub = 0;													 //开灯
		///////////////////change data end///////////////////
		memcpy(&adv_packet.date, &adv_date, sizeof(adv_packet.date));
		bls_ll_setAdvData((u8 *)&adv_packet, sizeof(adv_packet)); //设置广播数据
		bls_ll_setAdvEnable(1); 
		bls_ll_setAdvDuration(100*1000,1);
	}
	else
	{
		///////////////////change data start///////////////////
		memcpy(&adv_date.dst_mac_adr, all_device_adr, sizeof(adv_date.dst_mac_adr)); //数据：目标地址
		adv_date.op_code = 0x80;													 //开关灯命令
		adv_date.op_code_sub = 1;													 //关灯
		///////////////////change data end///////////////////
		memcpy(&adv_packet.date, &adv_date, sizeof(adv_packet.date));
		bls_ll_setAdvData((u8 *)&adv_packet, sizeof(adv_packet)); //设置广播数据
		bls_ll_setAdvEnable(1); 
		bls_ll_setAdvDuration(100*1000,1);
	}
}
void bsl_adv_remote_state( fun_control_sm_t state )
{
	if (state == CONTROL)
	{
		///////////////////change data start///////////////////
		memcpy(&adv_date.dst_mac_adr, all_device_adr, sizeof(adv_date.dst_mac_adr)); //数据：目标地址
		adv_date.op_code = 0x80;													 //开关灯命令
		// adv_date.op_code_sub = 0;													 //开灯
		///////////////////change data end///////////////////
		memcpy(&adv_packet.date, &adv_date, sizeof(adv_packet.date));
		bls_ll_setAdvData((u8 *)&adv_packet, sizeof(adv_packet)); //设置广播数据
	}
	else
	{
		///////////////////change data start///////////////////
		memcpy(&adv_date.dst_mac_adr, all_device_adr, sizeof(adv_date.dst_mac_adr)); //数据：目标地址
		adv_date.op_code = OPCODE_LED_BOUND_INQUIRE;													 //开关灯命令
		///////////////////change data end///////////////////
		memcpy(&adv_packet.date, &adv_date, sizeof(adv_packet.date));
		bls_ll_setAdvData((u8 *)&adv_packet, sizeof(adv_packet)); //设置广播数据
	}
}

void bsl_adv_led_all_bound(u8 on)
{
	if (on)
	{
		///////////////////change data start///////////////////
		memcpy(&adv_date.dst_mac_adr, all_device_adr, sizeof(adv_date.dst_mac_adr)); //数据：目标地址
		adv_date.op_code = OPCODE_LED_BOUND;										 //开关灯命令
		///////////////////change data end///////////////////
		memcpy(&adv_packet.date, &adv_date, sizeof(adv_packet.date));
		bls_ll_setAdvData((u8 *)&adv_packet, sizeof(adv_packet)); //设置广播数据
	}
	else
	{
		///////////////////change data start///////////////////
		memcpy(&adv_date.dst_mac_adr, all_device_adr, sizeof(adv_date.dst_mac_adr)); //数据：目标地址
		adv_date.op_code = OPCODE_LED_UNBOUND;										 //关灯
		///////////////////change data end///////////////////
		memcpy(&adv_packet.date, &adv_date, sizeof(adv_packet.date));
		bls_ll_setAdvData((u8 *)&adv_packet, sizeof(adv_packet)); //设置广播数据
	}
}

void bsl_adv_one_bound(u8 (*mac)[6], u8 bound, u8 cmd)
{
	memcpy(&adv_date.dst_mac_adr, mac, sizeof(adv_date.dst_mac_adr)); //数据：目标地址
	if (bound)														  //1绑定0解绑
	{
		if (cmd) //1确认0测试
		{
			adv_date.op_code = OPCODE_LED_BOUND_ONE;
		}
		else
		{
			adv_date.op_code = OPCODE_LED_BOUND_TEXT_ONE;
		}
	}
	else
	{
		if (cmd) //1确认0测试
		{
			adv_date.op_code = OPCODE_LED_UNBOUND_ONE;
		}
		else
		{
			adv_date.op_code = OPCODE_LED_BOUND_TEXT_ONE;
		}
	}
	memcpy(&adv_packet.date, &adv_date, sizeof(adv_packet.date));
	bls_ll_setAdvData((u8 *)&adv_packet, sizeof(adv_packet)); //设置广播数据
}

int bsl_adv_process(void)
{

	return 0;
}


#elif (DEVICE_TYPE == LIGHT)

void bsl_change_led_state_adv(u8 state)
{
	///////////////////change data start///////////////////
	if (state)
		adv_date.led_state = 0x01;
	else
		adv_date.led_state = 0x00;
	// ///////////////////change data end///////////////////
	memcpy(&adv_packet.date, &adv_date, sizeof(adv_packet.date));
	bls_ll_setAdvData((u8 *)&adv_packet, sizeof(adv_packet)); //设置广播数据
}


void bsl_iequire_bound_state_ack(u8 *remote_mac,u8 bound_state)
{
	///////////////////change data start///////////////////
	memcpy(&adv_date.dst_mac_adr, &scan_date.src_mac_adr, sizeof(adv_date.dst_mac_adr));
	adv_date.bound_state = bound_state;
	///////////////////change data end///////////////////
	memcpy(&adv_packet.date, &adv_date, sizeof(adv_packet.date));
	bls_ll_setAdvData((u8 *)&adv_packet, sizeof(adv_packet)); //设置广播数据
}


u8 bsl_is_bound_mac( u8 *mac_src)
{
	for (u8 i = 0; i < 10; i++)
    {
        if ((memcmp(&bound_remote_list[i][0], mac_src, 6) == 0))
            return i;
	}
	return 0xff;
}

u8 bsl_bound_mac_count( void)
{
	for (u8 i = 0; i < 10; i++)
    {
        if ( bound_remote_list[i][0] == 0)
            return i;
	}
	return 0xff;
}

u8 device_adv_name[5] = {'L', 'I', 'G', 'H', 'T'};

void bsl_adv_init(void)
{
	adv_packet.adv_flag_type = GAP_ADTYPE_FLAGS;
	adv_packet.adv_flag = 0x06;
	adv_packet.adv_flag_len = 2;

	adv_packet.name_type = GAP_ADTYPE_LOCAL_NAME_COMPLETE;
	adv_packet.name[0] = 'L';
	adv_packet.name[1] = 'I';
	adv_packet.name[2] = 'G';
	adv_packet.name[3] = 'H';
	adv_packet.name[4] = 'T';
	adv_packet.name_len = 6;

	adv_packet.device_type = GAP_ADTYPE_DEVICE_ID;
	adv_packet.device_id = 0x66;
	adv_packet.device_len = 2;

	adv_packet.date_type = GAP_ADTYPE_MANUFACTURER_SPECIFIC;

	memcpy(&adv_date.src_mac_adr, device_mac_adr, sizeof(adv_date.src_mac_adr)); //数据；源地址

	///////////////////change data start///////////////////

	adv_date.bound_state = bsl_bound_mac_count();

	if (global_light_state == 1)
	{
		device_led_setup(led_cfg[LIGHT_LED_ON]);
		adv_date.led_state = 0x01;
	}
	else
	{
		adv_date.led_state = 0x00;
	}
	

	///////////////////change data end///////////////////

	memcpy(&adv_packet.date, &adv_date, sizeof(adv_packet.date)); //数据给广播包
	adv_packet.date_len = sizeof(adv_packet.date) + 1;

	bls_ll_setAdvData((u8 *)&adv_packet, sizeof(adv_packet)); //设置广播数据
	//blt_soft_timer_add(bsl_adv_process, 10 * 1000);			  //100ms
}

int bsl_adv_process(void)
{
	if (memcmp(&scan_date.dst_mac_adr, all_device_adr, 6) == 0) //发给所有设备的消息：目标地址都是0xff
	{
		if (scan_date.op_code == OPCODE_LED_ONOFF)
		{
			if (bsl_is_bound_mac(&scan_date.src_mac_adr) != 0xff)//被该遥控器绑定
			{
				if (scan_date.op_code_sub == LED_ON)
				{
					global_light_state = 1;
					device_led_setup(led_cfg[LIGHT_LED_ON]); //开灯
					bsl_change_led_state_adv(LED_ON);		 //改变广播状态
					
					//tinyFlash_Write(STORAGE_LIGHT_STATE, &global_light_state, sizeof(global_light_state));
				}
				else if (scan_date.op_code_sub == LED_OFF)
				{
					global_light_state = 0;
					device_led_setup(led_cfg[LIGHT_LED_OFF]); //关灯
					bsl_change_led_state_adv(LED_OFF);

					//tinyFlash_Write(STORAGE_LIGHT_STATE, &global_light_state, sizeof(global_light_state));
				}
			}
			//bsl_adv_retrans(&scan_date.src_mac_adr,all_device_adr,scan_date.op_code,scan_date.op_code_sub);
		}
		else if (scan_date.op_code == OPCODE_LED_BOUND)
		{
			if (bsl_is_bound_mac(&scan_date.src_mac_adr) == 0xff)//没有被该设备绑定
			{
				for (u8 i = 0; i < 10; i++)
				{
					if (bound_remote_list[i][0] == 0)
					{
						memcpy(&bound_remote_list[i][0], &scan_date.src_mac_adr, 6);
						tinyFlash_Write(STORAGE_BOUND_MAC, (unsigned char *)bound_remote_list, sizeof(bound_remote_list));
						return;
					}
				}
			}
		}
		else if (scan_date.op_code == OPCODE_LED_UNBOUND)
		{	
			u8 cnt=0;		
			cnt=bsl_is_bound_mac(&scan_date.src_mac_adr);
			if ( cnt!= 0xff)//被该设备绑定
			{
				for (u8 i = 0; i < 6; i++)
				{
					bound_remote_list[cnt][i] = 0x00;
				}
				tinyFlash_Write(STORAGE_BOUND_MAC, (unsigned char *)bound_remote_list, sizeof(bound_remote_list));
			}
		}
		else if (scan_date.op_code == OPCODE_LED_BOUND_INQUIRE)//查询绑定状态
		{
			static u32 bsl_adv_delay = 0;
			if( clock_time_exceed(bsl_adv_delay, 10*1000))
			{
				bsl_adv_delay = clock_time();
				if (bsl_is_bound_mac(&scan_date.src_mac_adr) == 0xff)
				{
					bsl_iequire_bound_state_ack(&scan_date.src_mac_adr,0x00);
				}
				else
				{
					bsl_iequire_bound_state_ack(&scan_date.src_mac_adr,0x01);
				}
				
			}
		}
		
	}

/////////////////////////////////////////////////////////////////////////////////////////////


	else if (memcmp(&scan_date.dst_mac_adr, device_mac_adr, 6) == 0)//发给单个设备的消息：目标地址都是自身地址
	{
		if (scan_date.op_code == OPCODE_LED_BOUND_TEXT_ONE)
		{
			device_led_setup(led_cfg[LIGHT_LED_SELECT]);
		}
		else if (scan_date.op_code == OPCODE_LED_BOUND_ONE)
		{
			if (bsl_is_bound_mac(&scan_date.src_mac_adr) == 0xff)
			{
				for (u8 i = 0; i < 10; i++)
				{
					if (bound_remote_list[i][0] == 0)
					{
						memcpy(&bound_remote_list[i][0], &scan_date.src_mac_adr, 6);
						tinyFlash_Write(STORAGE_BOUND_MAC, (unsigned char *)bound_remote_list, sizeof(bound_remote_list));
						device_led_setup(led_cfg[LIGHT_LED_BOUND_ACK]);
						return;
					}
				}
			}

		}
		else if (scan_date.op_code == OPCODE_LED_UNBOUND_ONE)
		{
			u8 cnt=0;
			cnt=bsl_is_bound_mac(&scan_date.src_mac_adr);
			if ( cnt!= 0xff)//被该设备绑定
			{
				for (u8 i = 0; i < 6; i++)
				{
					bound_remote_list[cnt][i] = 0x00;
				}
				tinyFlash_Write(STORAGE_BOUND_MAC, (unsigned char *)bound_remote_list, sizeof(bound_remote_list));
				device_led_setup(led_cfg[LIGHT_LED_UNBOUND_ACK]);
			}
		}
	}

	return 0;
}

#endif

void bsl_adv_recive_data(u8 *data, u32 len)
{
		scan_date.src_mac_adr[0] = data[15];
		scan_date.src_mac_adr[1] = data[16];
		scan_date.src_mac_adr[2] = data[17];
		scan_date.src_mac_adr[3] = data[18];
		scan_date.src_mac_adr[4] = data[19];
		scan_date.src_mac_adr[5] = data[20];

		scan_date.dst_mac_adr[0] = data[21];
		scan_date.dst_mac_adr[1] = data[22];
		scan_date.dst_mac_adr[2] = data[23];
		scan_date.dst_mac_adr[3] = data[24];
		scan_date.dst_mac_adr[4] = data[25];
		scan_date.dst_mac_adr[5] = data[26];

		scan_date.op_code = data[27];
		scan_date.op_code_sub = data[28];
		scan_date.led_state = data[29];
		scan_date.bound_state = data[30];

	#if (DEVICE_TYPE == REMOTE)
		if (fun_control_sm == BOUND && scan_date.bound_state == 0 &&(memcmp(&scan_date.dst_mac_adr, device_mac_adr, 6) == 0) )
		{
			bsl_add(&scan_date.src_mac_adr, sizeof(scan_date.src_mac_adr));
		}
		else if (fun_control_sm == UNBOUND && scan_date.bound_state == 1 &&  (memcmp(&scan_date.dst_mac_adr, device_mac_adr, 6) == 0))
		{
			bsl_delate(&scan_date.src_mac_adr, sizeof(scan_date.src_mac_adr));
		}
	#endif
		bsl_adv_process();
}
