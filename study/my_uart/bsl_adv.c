#include "bsl_adv.h"
#include "fun_control.h"
#include "tl_common.h"
#include "drivers.h"
#include "global_event_queue.h"
#include "vendor/common/blt_soft_timer.h"
#include "vendor/common/blt_led.h"
#include <stack/ble/ble.h>

adv_packet_t adv_packet={0};
adv_date_t	adv_date={0};
scan_date_t scan_date={0};
u8 all_device_adr[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
int bsl_adv_process(void);


bool bsl_is_macadr(u8 *inmacadr,u8 *selfmacadr)
{
	if ((inmacadr[0]	== selfmacadr[0]&& inmacadr[1]	== selfmacadr[1]&& \
	 inmacadr[2]	== selfmacadr[2]&& inmacadr[3]	== selfmacadr[3]&& \
	 inmacadr[4]	== selfmacadr[4]&& inmacadr[5]	== selfmacadr[5])|| \
	 (inmacadr[0]	== all_device_adr[0]&& inmacadr[1]	== all_device_adr[1]&& \
	 inmacadr[2]	== all_device_adr[2]&& inmacadr[3]	== all_device_adr[3]&& \
	 inmacadr[4]	== all_device_adr[4]&& inmacadr[5]	== all_device_adr[5]))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void bsl_change_led_state_adv(u8 state)
{
	///////////////////change data start///////////////////
	if (state)
		adv_date.led_state=0x01;
	else
		adv_date.led_state=0x00;
	///////////////////change data end///////////////////
	memcpy(adv_packet.date, &adv_date, sizeof(adv_packet.date));
	bls_ll_setAdvData( (u8 *)&adv_packet, sizeof(adv_packet) ); //设置广播数据
}


// bound_mac_adr





#if (DEVICE_TYPE == REMOTE)

u8 device_adv_name[5]={'R','E','M','O','T'};

void bsl_adv_init(void)
{
	adv_packet.adv_flag_type=GAP_ADTYPE_FLAGS;
	adv_packet.adv_flag=0x06;
	adv_packet.adv_flag_len=2;

	adv_packet.name_type=GAP_ADTYPE_LOCAL_NAME_COMPLETE;
	memcpy(adv_packet.name, device_adv_name, sizeof(adv_packet.name));
	adv_packet.name_len=sizeof(adv_packet.name)+1;

	adv_packet.device_type=GAP_ADTYPE_DEVICE_ID;
	adv_packet.device_id=0x66;
	adv_packet.device_len=2;

	adv_packet.date_type=GAP_ADTYPE_MANUFACTURER_SPECIFIC;
	
	memcpy(adv_date.src_mac_adr, device_mac_adr, sizeof(adv_date.src_mac_adr));//数据；源地址

	///////////////////change data start///////////////////
	memcpy(adv_date.dst_mac_adr, all_device_adr, sizeof(adv_date.dst_mac_adr));//数据：目标地址
	adv_date.op_code=0x80;//开关灯命令
	adv_date.op_code_sub=0;//开机关灯

	///////////////////change data end///////////////////


	memcpy(adv_packet.date, &adv_date, sizeof(adv_packet.date));
	
	adv_packet.date_len=sizeof(adv_packet.date)+1;

    bls_ll_setAdvData( (u8 *)&adv_packet, sizeof(adv_packet) ); //设置广播数据
	blt_soft_timer_add(bsl_adv_process,100*1000);//100ms
}




void bsl_adv_led_onoff(u8 on)
{
	if (on)
	{
		///////////////////change data start///////////////////
		memcpy(adv_date.dst_mac_adr, all_device_adr, sizeof(adv_date.dst_mac_adr));//数据：目标地址
		adv_date.op_code=0x80;//开关灯命令
		adv_date.op_code_sub=0;//开灯
		///////////////////change data end///////////////////
		memcpy(adv_packet.date, &adv_date, sizeof(adv_packet.date));
		bls_ll_setAdvData( (u8 *)&adv_packet, sizeof(adv_packet) ); //设置广播数据
	}
	else
	{
		///////////////////change data start///////////////////
		memcpy(adv_date.dst_mac_adr, all_device_adr, sizeof(adv_date.dst_mac_adr));//数据：目标地址
		adv_date.op_code=0x80;//开关灯命令
		adv_date.op_code_sub=1;//关灯
		///////////////////change data end///////////////////
		memcpy(adv_packet.date, &adv_date, sizeof(adv_packet.date));
		bls_ll_setAdvData( (u8 *)&adv_packet, sizeof(adv_packet) ); //设置广播数据
	}
}

int bsl_adv_process(void)
{
	
	

	return 0;
}


#elif(DEVICE_TYPE == LIGHT)


u8 device_adv_name[5]={'L','I','G','H','T'};

void bsl_adv_init(void)
{
	adv_packet.adv_flag_type=GAP_ADTYPE_FLAGS;
	adv_packet.adv_flag=0x06;
	adv_packet.adv_flag_len=2;

	adv_packet.name_type=GAP_ADTYPE_LOCAL_NAME_COMPLETE;
	adv_packet.name[0]='L';
	adv_packet.name[1]='I';
	adv_packet.name[2]='G';
	adv_packet.name[3]='H';
	adv_packet.name[4]='T';
	adv_packet.name_len=6;

	adv_packet.device_type=GAP_ADTYPE_DEVICE_ID;
	adv_packet.device_id=0x66;
	adv_packet.device_len=2;

	adv_packet.date_type=GAP_ADTYPE_MANUFACTURER_SPECIFIC;
	memcpy(adv_date.src_mac_adr, device_mac_adr, sizeof(adv_date.src_mac_adr));//数据；源地址

	///////////////////change data start///////////////////
	memcpy(adv_date.dst_mac_adr, all_device_adr, sizeof(adv_date.dst_mac_adr));//数据：目标地址
	
	adv_date.led_state=0x00;
	adv_date.bound_state=0x00;
	

	///////////////////change data end///////////////////


	memcpy(adv_packet.date, &adv_date, sizeof(adv_packet.date));//数据给广播包
	adv_packet.date_len=sizeof(adv_packet.date)+1;

    bls_ll_setAdvData( (u8 *)&adv_packet, sizeof(adv_packet) ); //设置广播数据
	blt_soft_timer_add(bsl_adv_process,100*1000);//100ms

}


int bsl_adv_process(void)
{
	if (bsl_is_macadr(&scan_date.dst_mac_adr,device_mac_adr))//源地址为绑定地址
	{
		if (scan_date.op_code == OPCODE_LED_ONOFF)
		{
			if (scan_date.op_code_sub == LED_ON)
			{
				device_led_setup(led_cfg[LIGHT_LED_ON]);//开灯
				bsl_change_led_state_adv(LED_ON);//改变广播状态
			}
			else if (scan_date.op_code_sub == LED_OFF )
			{
				device_led_setup(led_cfg[LIGHT_LED_OFF]);//关灯
				bsl_change_led_state_adv(LED_OFF);
			}
		}
		else if(scan_date.op_code == OPCODE_LED_BOUND)
		{


		
		}
		else if (scan_date.op_code == OPCODE_LED_UNBOUND)
		{



		
		}
	}
	return 0;
}


#endif

	



void bsl_adv_recive_data(u8 * data, u32 len)
{
	scan_date.src_mac_adr[0]=data[15];
	scan_date.src_mac_adr[1]=data[16];
	scan_date.src_mac_adr[2]=data[17];
	scan_date.src_mac_adr[3]=data[18];
	scan_date.src_mac_adr[4]=data[19];
	scan_date.src_mac_adr[5]=data[20];

	scan_date.dst_mac_adr[0]=data[21];
	scan_date.dst_mac_adr[1]=data[22];
	scan_date.dst_mac_adr[2]=data[23];
	scan_date.dst_mac_adr[3]=data[24];
	scan_date.dst_mac_adr[4]=data[25];
	scan_date.dst_mac_adr[5]=data[26];

	scan_date.op_code=data[27];
	scan_date.op_code_sub=data[28];
	scan_date.led_state=data[29];
	scan_date.bound_state=data[30];
}










