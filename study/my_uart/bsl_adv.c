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

u8 all_device_adr[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

#if (DEVICE_TYPE == REMOTE)

u8 device_adv_name[5]={'R','E','M','O','T'};


#elif(DEVICE_TYPE == LIGHT)

u8 device_adv_name[5]={'L','I','G','H','T'};

#endif


void bsl_adv_init(void)
{

#if (DEVICE_TYPE == REMOTE)

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


#elif(DEVICE_TYPE == LIGHT)

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



#endif


}

#if (DEVICE_TYPE == REMOTE)

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

#elif(DEVICE_TYPE == LIGHT)




#endif


















