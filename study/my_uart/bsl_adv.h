
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

typedef struct{//16byte
    u8 src_mac_adr[6];
	u8 dst_mac_adr[6];
    u8 op_code;
    u8 op_code_sub;
    u8 led_state;
    u8 bound_state;
}scan_date_t;


extern u8  device_mac_adr[6];
extern u8  bound_mac_adr[6];


#define OPCODE_LED_ONOFF 0x80
#define LED_ON 0x01
#define LED_OFF 0
#define OPCODE_LED_BOUND 0x81
#define OPCODE_LED_UNBOUND 0x82





void bsl_adv_init(void);
void bsl_adv_led_onoff(u8 on);
void bsl_adv_recive_data(u8 * data, u32 len);
void bsl_adv_led_all_bound(u8 on);
void bsl_adv_add_callback(void (*add)(u8 *mac));


#endif



