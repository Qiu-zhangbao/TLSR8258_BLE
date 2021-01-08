
#ifndef __BSL_ADV__
#define __BSL_ADV__

#include "tl_common.h"
#include "drivers.h"

#define ADV_RECE_FIFO_LEN      40
#define ADV_SEND_FIFO_LEN      40
#define ADV_PACK_LEN           28

#define AES_ENABLE           1

#define adv_fifo_iptr_plus(a,b)     ((a+b)%ADV_RECE_FIFO_LEN)
#define adv_fifo_optr_plus(a,b)     ((a+b)%ADV_SEND_FIFO_LEN)
#define adv_send_seq_plus(a,b)      ((a+b)&0xFFFF)

#define ADV_CMD_RETRYTIMES     10


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
        uint8_t local_mac[6];
        uint8_t local_cmd[2];
        uint8_t cmd_load[8];
        uint8_t count;
    }item;
    uint8_t adv_array[ADV_PACK_LEN+1];
}adv_send_data_t;

#define ADV_PACK_TTL                1


extern u8  device_mac_adr[6];

void bsl_adv_init(void);
void bsl_adv_recive_data(u8 * data, u32 len);
void adv_vendor_send_cmd(uint16_t user_cmd,uint8_t *pack_load,uint8_t len,uint8_t ttl);




#endif



