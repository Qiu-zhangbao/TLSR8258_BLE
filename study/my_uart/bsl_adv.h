
#ifndef __BSL_ADV__
#define __BSL_ADV__


#include "tl_common.h"


typedef struct{

    u8 adv_flag_len;
    u8 adv_flag_type;
    u8 adv_flag;

	u8 name_len;
	u8 name_type;
    u8 name[5];

    u8 device_len;
    u8 device_type;
    u8 device_id;

    u8 date_len;
    u8 date_type;
    u8 date[16];

}adv_packet_t;


typedef struct{//16byte
    u8 src_mac_adr[6];
	u8 dst_mac_adr[6];
    u8 op_code;
    u8 op_code_sub;
    u8 led_state;
    u8 bound_state;
}adv_date_t;


extern u8  device_mac_adr[6];









void bsl_adv_init(void);
void bsl_adv_led_onoff(u8 on);



#endif



