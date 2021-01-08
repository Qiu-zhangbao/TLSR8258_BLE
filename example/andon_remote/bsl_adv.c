#include "bsl_adv.h"
#include "stack/ble/ble.h"
#include "AES.h"



static adv_rece_data_t adv_fifo_in[ADV_RECE_FIFO_LEN];
static adv_send_data_t adv_fifo_out[ADV_SEND_FIFO_LEN];
static uint16_t   adv_fifo_in_iptr = 0;
static uint16_t   adv_fifo_out_optr = 0;
static uint16_t   adv_fifo_out_iptr = 0;


#if 0
const uint8_t adv_send_data_head[] = {ADV_PACK_LEN-1,0xff,0x04,0x08,'W','y','z'};
#else
const uint8_t adv_send_data_head[] = {ADV_PACK_LEN-1,0xff,0x04,0x08,'q','z','b'};
#endif

uint32_t   adv_send_seq = 0xFD;
uint8_t remote_index = 0;
const uint8_t aes128key[16] = "com.jiuan.SLight";

void bsl_adv_init(void)
{
	#if AES_ENABLE
    AES_Init(aes128key);
#endif // AES_ENABLE
// 	adv_packet.adv_flag_type = GAP_ADTYPE_FLAGS;
// 	adv_packet.adv_flag = 0x06;
// 	adv_packet.adv_flag_len = 2;

// 	adv_packet.name_type = GAP_ADTYPE_LOCAL_NAME_COMPLETE;
// 	memcpy(adv_packet.name, device_adv_name, sizeof(adv_packet.name));
// 	adv_packet.name_len = sizeof(adv_packet.name) + 1;

// 	adv_packet.device_type = GAP_ADTYPE_DEVICE_ID;
// 	adv_packet.device_id = 0x66;
// 	adv_packet.device_len = 2;

// 	adv_packet.date_type = GAP_ADTYPE_MANUFACTURER_SPECIFIC;

// 	memcpy(&adv_date.src_mac_adr, device_mac_adr, sizeof(adv_date.src_mac_adr)); //数据；源地址

// 	///////////////////change data start///////////////////
// 	memcpy(&adv_date.dst_mac_adr, all_device_adr, sizeof(adv_date.dst_mac_adr)); //数据：目标地址
// 	adv_date.op_code = 0x80;													 //开关灯命令
// 	adv_date.op_code_sub = 0;			
// 	adv_date.led_state = 0xff;
// 	adv_date.bound_state = 0xff;

// 	///////////////////change data end///////////////////

// 	memcpy(&adv_packet.date, &adv_date, sizeof(adv_packet.date));

// 	adv_packet.date_len = sizeof(adv_packet.date) + 1;

// 	bls_ll_setAdvData((u8 *)&adv_packet, sizeof(adv_packet)); //设置广播数据
// 	//blt_soft_timer_add(bsl_adv_process, 10 * 1000);			  //100ms
}


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



void adv_vendor_send_cmd(uint16_t user_cmd,uint8_t *pack_load,uint8_t len,uint8_t ttl)
{
    uint16_t length = 0;
    uint32_t seed;
    uint16_t crc_send;
    uint8_t own_bd_addr[6];
    uint8_t  mac_index=0;

    // WICED_LOG_DEBUG("Send cmd: 0x%02x\r\n",user_cmd);
    memcpy(adv_fifo_out[adv_fifo_out_iptr].adv_array,adv_send_data_head,sizeof(adv_send_data_head));

    adv_fifo_out[adv_fifo_out_iptr].item.seq = (uint8_t)(adv_send_seq&0xFF);
    adv_fifo_out[adv_fifo_out_iptr].item.segment = 0;
    adv_send_seq = adv_send_seq_plus(adv_send_seq,1);
    adv_fifo_out[adv_fifo_out_iptr].item.ttl = ttl;
    
    memcpy(adv_fifo_out[adv_fifo_out_iptr].item.local_mac, device_mac_adr,sizeof(device_mac_adr));


    mac_index = *(uint8_t *)(adv_fifo_out[adv_fifo_out_iptr].item.local_mac);
    mac_index = (mac_index&0x3F) | (remote_index<<6);
    *(uint8_t *)(adv_fifo_out[adv_fifo_out_iptr].item.local_mac) = mac_index;
    adv_fifo_out[adv_fifo_out_iptr].item.local_cmd[0] = (uint8_t)((user_cmd>>8)&0xFF);
    adv_fifo_out[adv_fifo_out_iptr].item.local_cmd[1] = (uint8_t)(user_cmd);

    //TODO 暂不做分包处理
    if(len > sizeof(adv_fifo_out[adv_fifo_out_iptr].item.cmd_load))
        len = sizeof(adv_fifo_out[adv_fifo_out_iptr].item.cmd_load);
    memcpy(adv_fifo_out[adv_fifo_out_iptr].item.cmd_load,pack_load,len);

    generateRandomNum(sizeof(adv_fifo_out[adv_fifo_out_iptr].item.cmd_load)-len,adv_fifo_out[adv_fifo_out_iptr].item.cmd_load+len);

    //数据混淆处理
    seed = adv_fifo_out[adv_fifo_out_iptr].item.seq;
    for (int i = sizeof(adv_send_data_head)+4; i < (sizeof(adv_fifo_out[adv_fifo_out_iptr].adv_array)-1); i++)
    {
        seed = 214013 * seed + 2531011;
        adv_fifo_out[adv_fifo_out_iptr].adv_array[i] ^= (seed >> 16) & 0xff;
    }
    //校验
    crc_send = CrcCalc(adv_fifo_out[adv_fifo_out_iptr].adv_array+(sizeof(adv_send_data_head)+3),
                       sizeof(adv_fifo_out[adv_fifo_out_iptr].adv_array)-(sizeof(adv_send_data_head)+4));
    adv_fifo_out[adv_fifo_out_iptr].item.crc_check[0] = (uint8_t)(crc_send>>8);
    adv_fifo_out[adv_fifo_out_iptr].item.crc_check[1] = (uint8_t)(crc_send);
#if AES_ENABLE
    //AES加密
    AES_Encrypt_S(adv_fifo_out[adv_fifo_out_iptr].item.local_mac,adv_fifo_out[adv_fifo_out_iptr].item.local_mac,16,aes128key);
#endif
    adv_fifo_out[adv_fifo_out_iptr].item.count = ADV_CMD_RETRYTIMES;
    //adv_fifo_out_iptr = adv_fifo_optr_plus(adv_fifo_out_iptr,1);
    // adv_send_Scheduling();
	bls_ll_setAdvData((u8 *)&adv_fifo_out[adv_fifo_out_iptr].adv_array,ADV_PACK_LEN); 
	bls_ll_setAdvEnable(1); 
	bls_ll_setAdvDuration(100*1000,1);
}




void bsl_adv_recive_data(u8 * data, u32 len)
{




}








