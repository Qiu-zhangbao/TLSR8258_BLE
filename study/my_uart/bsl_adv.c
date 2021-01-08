#include "bsl_adv.h"
#include "fun_control.h"
#include "tl_common.h"
#include "drivers.h"
#include "global_event_queue.h"
#include "vendor/common/blt_soft_timer.h"
#include "vendor/common/blt_led.h"
#include <stack/ble/ble.h>
#include "tinyFlash/tinyFlash.h"
#include "app_uart.h"
#include "app_config.h"
adv_packet_t adv_packet;
adv_date_t adv_date;
scan_date_t scan_date;

u8 all_device_adr[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

const uint8_t aes128key[16] = "com.jiuan.SLight";

#if (DEVICE_TYPE == REMOTE)

int bsl_adv_process(void);

void (*bsl_add)(u8 (*mac)[6], u8 len);
void (*bsl_delate)(u8 (*mac)[6],u8 len);

u8 device_adv_name[5] = {'R', 'E', 'M', 'O', 'T'};



void bsl_adv_init(void)
{
	
	// AES_Init();
	// adv_packet.adv_flag_type = GAP_ADTYPE_FLAGS;
	// adv_packet.adv_flag = 0x06;
	// adv_packet.adv_flag_len = 2;

	// adv_packet.name_type = GAP_ADTYPE_LOCAL_NAME_COMPLETE;
	// memcpy(adv_packet.name, device_adv_name, sizeof(adv_packet.name));
	// adv_packet.name_len = sizeof(adv_packet.name) + 1;

	// adv_packet.device_type = GAP_ADTYPE_DEVICE_ID;
	// adv_packet.device_id = 0x66;
	// adv_packet.device_len = 2;

	// adv_packet.date_type = GAP_ADTYPE_MANUFACTURER_SPECIFIC;

	// memcpy(&adv_date.src_mac_adr, device_mac_adr, sizeof(adv_date.src_mac_adr)); //数据；源地址

	// ///////////////////change data start///////////////////
	// memcpy(&adv_date.dst_mac_adr, all_device_adr, sizeof(adv_date.dst_mac_adr)); //数据：目标地址
	// adv_date.op_code = 0x80;													 //开关灯命令
	// adv_date.op_code_sub = 0;			
	// adv_date.led_state = 0xff;
	// adv_date.bound_state = 0xff;

	// ///////////////////change data end///////////////////

	// memcpy(&adv_packet.date, &adv_date, sizeof(adv_packet.date));

	// adv_packet.date_len = sizeof(adv_packet.date) + 1;

	// bls_ll_setAdvData((u8 *)&adv_packet, sizeof(adv_packet)); //设置广播数据
	// blt_soft_timer_add(bsl_adv_process, 10 * 1000);			  //100ms
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
void bsl_adv_sned_qedc(int qedc)
{
	adv_date.bound_state = qedc&0xff;

	memcpy(&adv_packet.date, &adv_date, sizeof(adv_packet.date));
	bls_ll_setAdvData((u8 *)&adv_packet, sizeof(adv_packet)); //设置广播数据
	bls_ll_setAdvEnable(1); 
	bls_ll_setAdvDuration(150*1000,1);

}

int bsl_adv_process(void)
{

	return 0;
}


#elif (DEVICE_TYPE == LIGHT)


int bsl_light_time_handle(void);

u8 bsl_light_time_save_data=0;

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
	if ((memcmp(&adv_date.dst_mac_adr, remote_mac, 6) != 0)||adv_date.bound_state!=bound_state)
	{
		///////////////////change data start///////////////////
		memcpy(&adv_date.dst_mac_adr, remote_mac, sizeof(adv_date.dst_mac_adr));
		adv_date.bound_state = bound_state;
		///////////////////change data end///////////////////
		memcpy(&adv_packet.date, &adv_date, sizeof(adv_packet.date));
		bls_ll_setAdvData((u8 *)&adv_packet, sizeof(adv_packet)); //设置广播数据

		if (bound_state)
			at_print("iequire bound state :1 \r\n");
		else
			at_print("iequire bound state :0 \r\n");
	}
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
	#if 1
	AES_Init(aes128key);
	#endif 
	// adv_packet.adv_flag_type = GAP_ADTYPE_FLAGS;
	// adv_packet.adv_flag = 0x06;
	// adv_packet.adv_flag_len = 2;

	// adv_packet.name_type = GAP_ADTYPE_LOCAL_NAME_COMPLETE;
	// adv_packet.name[0] = 'L';
	// adv_packet.name[1] = 'I';
	// adv_packet.name[2] = 'G';
	// adv_packet.name[3] = 'H';
	// adv_packet.name[4] = 'T';
	// adv_packet.name_len = 6;

	// adv_packet.device_type = GAP_ADTYPE_DEVICE_ID;
	// adv_packet.device_id = 0x66;
	// adv_packet.device_len = 2;

	// adv_packet.date_type = GAP_ADTYPE_MANUFACTURER_SPECIFIC;

	// memcpy(&adv_date.src_mac_adr, device_mac_adr, sizeof(adv_date.src_mac_adr)); //数据；源地址

	// ///////////////////change data start///////////////////

	// adv_date.bound_state = bsl_bound_mac_count();

	// if (global_light_state == 1)
	// {
	// 	device_led_setup(led_cfg[LIGHT_LED_ON]);
	// 	adv_date.led_state = 0x01;
	// }
	// else
	// {
	// 	adv_date.led_state = 0x00;
	// }
	

	// ///////////////////change data end///////////////////

	// memcpy(&adv_packet.date, &adv_date, sizeof(adv_packet.date)); //数据给广播包
	// adv_packet.date_len = sizeof(adv_packet.date) + 1;

	// bls_ll_setAdvData((u8 *)&adv_packet, sizeof(adv_packet)); //设置广播数据
	// blt_soft_timer_add(bsl_light_time_handle, 1000 * 1000);			  //1s
}

u8 need_save_light_state=0;
u8 need_save_bound_state=0;

int bsl_light_time_handle(void)
{
	bsl_light_time_save_data++;
	if (bsl_light_time_save_data >5)
	{
		bsl_light_time_save_data=0;
		if (need_save_bound_state)
		{
			need_save_bound_state = 0;
			tinyFlash_Write(STORAGE_BOUND_MAC, (unsigned char *)bound_remote_list, sizeof(bound_remote_list));
		}
		
		if (need_save_light_state)
		{
			need_save_light_state = 0;
			tinyFlash_Write(STORAGE_LIGHT_STATE, &global_light_state, sizeof(global_light_state));
		}
		
		
	}
	return 0;
}



void bsl_adv_process(void)
{
	if (memcmp(&scan_date.dst_mac_adr, all_device_adr, 6) == 0) //发给所有设备的消息：目标地址都是0xff
	{
		if (scan_date.op_code == OPCODE_LED_ONOFF)
		{
			bsl_light_time_save_data=0;//操作标志
			if (bsl_is_bound_mac(&scan_date.src_mac_adr) != 0xff)//被该遥控器绑定
			{
				if (scan_date.op_code_sub == LED_ON)
				{
					if (global_light_state == 0)//当前关灯
					{
						global_light_state = 1;
						need_save_light_state = 1;
						bsl_light_time_save_data=0;//操作标志
						device_led_setup(led_cfg[LIGHT_LED_ON]); //开灯
						bsl_change_led_state_adv(LED_ON);		 //改变广播状态
						at_print("led on \r\n");
					}
				}
				else if (scan_date.op_code_sub == LED_OFF)
				{
					if (global_light_state == 1)//当前关灯
					{
						global_light_state = 0;
						need_save_light_state = 1;
						device_led_setup(led_cfg[LIGHT_LED_OFF]); //关灯
						bsl_change_led_state_adv(LED_OFF);
						at_print("led off \r\n");
					}
				}
			}
		}
		else if (scan_date.op_code == OPCODE_LED_BOUND)
		{
			if (bsl_is_bound_mac(&scan_date.src_mac_adr) == 0xff)//没有被该设备绑定
			{
				bsl_light_time_save_data=0;//操作标志
				for (u8 i = 0; i < 10; i++)
				{
					if (bound_remote_list[i][0] == 0)
					{
						memcpy(&bound_remote_list[i][0], &scan_date.src_mac_adr, 6);
						need_save_bound_state = 1;
						at_print("bound all \r\n");
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
				bsl_light_time_save_data=0;//操作标志
				for (u8 i = 0; i < 6; i++)
				{
					bound_remote_list[cnt][i] = 0x00;
				}
				need_save_bound_state = 1;
				at_print("unbound all \r\n");
			}
		}
		else if (scan_date.op_code == OPCODE_LED_BOUND_INQUIRE)//查询绑定状态
		{
			static u32 bsl_adv_delay = 0;
			bsl_light_time_save_data=0;//操作标志
			if( clock_time_exceed(bsl_adv_delay, 10*1000))
			{
				bsl_adv_delay = clock_time();
				if (bsl_is_bound_mac(&scan_date.src_mac_adr) == 0xff)//没绑定
				{
					bsl_iequire_bound_state_ack(&scan_date.src_mac_adr,0x00);
				}
				else//绑定了
				{
					bsl_iequire_bound_state_ack(&scan_date.src_mac_adr,0x01);
				}
			}
		}
		
	}

/////////////////////////////////////////////////////////////////////////////////////////////


	else if (memcmp(&scan_date.dst_mac_adr, device_mac_adr, 6) == 0)//发给单个设备的消息：目标地址都是自身地址
	{
		bsl_light_time_save_data=0;//操作标志
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
						device_led_setup(led_cfg[LIGHT_LED_BOUND_ACK]);
						need_save_bound_state = 1;
						at_print("bound one success \r\n");
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
				device_led_setup(led_cfg[LIGHT_LED_UNBOUND_ACK]);
				need_save_bound_state = 1;
				at_print("unbound one success \r\n");
			}
		}
	}
}

#endif

// void bsl_adv_recive_data(u8 *data, u32 len)
// {
// 	scan_date.src_mac_adr[0] = data[15];
// 	scan_date.src_mac_adr[1] = data[16];
// 	scan_date.src_mac_adr[2] = data[17];
// 	scan_date.src_mac_adr[3] = data[18];
// 	scan_date.src_mac_adr[4] = data[19];
// 	scan_date.src_mac_adr[5] = data[20];

// 	scan_date.dst_mac_adr[0] = data[21];
// 	scan_date.dst_mac_adr[1] = data[22];
// 	scan_date.dst_mac_adr[2] = data[23];
// 	scan_date.dst_mac_adr[3] = data[24];
// 	scan_date.dst_mac_adr[4] = data[25];
// 	scan_date.dst_mac_adr[5] = data[26];

// 	scan_date.op_code = data[27];
// 	scan_date.op_code_sub = data[28];
// 	scan_date.led_state = data[29];
// 	scan_date.bound_state = data[30];

// #if (DEVICE_TYPE == REMOTE)
// 	if (fun_control_sm == BOUND && scan_date.bound_state == 0 &&(memcmp(&scan_date.dst_mac_adr, device_mac_adr, 6) == 0) )
// 	{
// 		bsl_add(&scan_date.src_mac_adr, sizeof(scan_date.src_mac_adr));
// 	}
// 	else if (fun_control_sm == UNBOUND && scan_date.bound_state == 1 &&  (memcmp(&scan_date.dst_mac_adr, device_mac_adr, 6) == 0))
// 	{
// 		bsl_delate(&scan_date.src_mac_adr, sizeof(scan_date.src_mac_adr));
// 	}
// #endif
// 	bsl_adv_process();
// }



#define ADV_PACK_LEN           28

#if 0
const uint8_t adv_send_data_head[] = {ADV_PACK_LEN-1,0xff,0x04,0x08,'W','y','z'};
#else
const uint8_t adv_send_data_head[] = {ADV_PACK_LEN-1,0xff,0x04,0x08,'q','z','b'};
#endif


typedef union{
    struct
    {
        uint8_t adv_len;
        uint8_t adv_type;
        uint8_t companyid[2];
        uint8_t manu_name[3];
        uint8_t ttl;
        uint8_t crc_check[2];
        uint8_t seq;
        uint8_t segment;
        uint8_t remote_mac[6];
        uint8_t remote_cmd[2];
        uint8_t cmd_load[8];
    }item;
    uint8_t adv_array[ADV_PACK_LEN];
}adv_rece_data_t;


//*****************************************************************************
// 函数名称: CrcCalc
// 函数描述: 自定义广播的初始化--用于与灯的交互内容
// 函数输入:  
// 函数返回值: 
//*****************************************************************************/
uint16_t CrcCalc(uint8_t *data, uint16_t length)
{
    uint16_t i;
    uint8_t j;
    union
    {
        uint16_t CRCX;
        uint8_t CRCY[2];
    } CRC;

    CRC.CRCX = 0xFFFF;
    for(i=0; i<length; i++)
    {
        CRC.CRCY[0] = (CRC.CRCY[0] ^ data[i]);
        for(j=0; j<8; j++)
        {
            if((CRC.CRCX & 0X0001) == 1)
                CRC.CRCX = (CRC.CRCX >> 1) ^ 0X1021;
            else
                CRC.CRCX >>= 1;
        }
    }
    return CRC.CRCX;
}



#include "AES.h"

void bsl_adv_recive_data(u8 *p_adv_data, u32 len)
{
	adv_rece_data_t adv_data;
  
    uint16_t rece_cmd;

    uint16_t rece_crc;
    uint32_t seed;
 
    //数据头不匹配，不做解析
    if (0 != memcmp(p_adv_data, adv_send_data_head, sizeof(adv_send_data_head)))
    {
        return ;
    }
    //copy data
    memcpy(adv_data.adv_array, p_adv_data, ADV_PACK_LEN);

 	AES_Decrypt_S(adv_data.item.remote_mac, adv_data.item.remote_mac, 16, aes128key);

    rece_crc = adv_data.item.crc_check[0];
    rece_crc = rece_crc * 256 + adv_data.item.crc_check[1];
    if (rece_crc != CrcCalc(adv_data.adv_array + sizeof(adv_send_data_head)+3, 
                            sizeof(adv_data.adv_array) - (sizeof(adv_send_data_head)+3)))
    {
		at_print("crc");
		at_print("\r\n");
        return ;
    }

    seed = adv_data.item.seq;
    for (int i = sizeof(adv_send_data_head)+4; i < sizeof(adv_data.adv_array); i++)
    {
        seed = 214013 * seed + 2531011;
        adv_data.adv_array[i] ^= (seed >> 16) & 0xff;
    }
	at_print("\r\n");
	at_print_array(adv_data.adv_array, ADV_PACK_LEN);
	at_print("\r\n");
	
}


