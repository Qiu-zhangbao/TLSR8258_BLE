#include "fun_bound.h"
#include "global_event_queue.h"
#include "apc_fun_sm.h"
#include "vendor/common/blt_soft_timer.h"
#include "vendor/common/blt_led.h"
#include "app_config.h"
#include "bsl_adv.h"

static event_type_t fun_boundevent_handle(event_type_t event);

extern int qedc_value;
static inline void AndonPair_Wakeup(void);

void fun_bound_init(void)
{
    resgister_event_handle(fun_boundevent_handle,EVENT_ONOFF_ONE|EVENT_PAIR_IN|EVENT_ONOFF_LONG);
}


static inline void AndonPair_Wakeup(void)
{
    mesh_btn_pairload_t pack_load;
    pack_load.pair_stata = ADV_PARILOAD_WAKEUP;
    adv_vendor_send_cmd(ADV_CMD_PARI,(uint8_t*)&pack_load,sizeof(pack_load),0);
}
static inline void AndonPair_SendPairSelect(u8* dst_mac)
{
    mesh_btn_pairload_t pack_load;
    // WICED_LOG_DEBUG("Send Blink Cmd to %B \n",dst_mac);
    pack_load.pair_stata = ADV_PARILOAD_SELECT;
    memcpy(pack_load.dst_mac,dst_mac,6);
    adv_vendor_send_cmd(ADV_CMD_PARI,(uint8_t *)(&pack_load),sizeof(mesh_btn_pairload_t),1);
}

static inline void AndonPair_SendPairStart(void)
{
    uint8_t pack_load;
    pack_load = ADV_PARILOAD_DOPAIR;
    adv_vendor_send_cmd(ADV_CMD_PARI,(uint8_t*)&pack_load,sizeof(pack_load),0);
}


void AndonPair_Stop(void)
{
    uint8_t empty_mac[6] = {0};
    // AndonPair_handle.AndonPair_doing = WICED_FALSE;
    // AndonPair_handle.AndonPair_SelectLightFlag = WICED_FALSE;
    AndonPair_SendPairSelect(empty_mac);
    // WICED_LOG_DEBUG("blink mac %B  stop paired \r\n",empty_mac);
    // wiced_stop_timer(&AndonPair_handle.AndonPair_timer);
    // AndonPair_handle.AndonPair_timer_cnt = 0;
    // memset(&AndonPair_Cache,0,sizeof(mesh_paircache_t));
}

static inline void AndonPair_SendPaired(u8* dst_mac)
{
    mesh_btn_pairload_t pack_load;

    // WICED_LOG_DEBUG("Send Config Cmd to %B \n",dst_mac);
    pack_load.pair_stata = ADV_PARILOAD_PAIRED;
    memcpy(pack_load.dst_mac,dst_mac,6);
    adv_vendor_send_cmd(ADV_CMD_PARI,(uint8_t *)(&pack_load),sizeof(mesh_btn_pairload_t),0);
}


int fun_bound_process(void)
{
   
    return 0;
}

void fun_bound_in(void)
{
    device_led_setup(led_cfg[REMOTE_LED_BOUND_MODE]);
    AndonPair_Wakeup();
}

void fun_bound_out(void)
{
    device_led_setup(led_cfg[REMOTE_LED_KEY_PRESS]);
    AndonPair_Stop();
}

u8 mac_dst[6]={0xEB,0x3A,0x96,0xAF,0x43,0x54};

static event_type_t fun_boundevent_handle(event_type_t event)
{
    if (apc_sm == BOUND)
    {
        switch (event)
        {
        case EVENT_ONOFF_ONE: //选灯
            AndonPair_SendPairSelect(mac_dst);
            break;
        case EVENT_PAIR_IN:
            apc_sm = CONTROL;
            fun_control_sm_mgr(apc_sm);
            break; 
        case EVENT_ONOFF_LONG: //选灯
            AndonPair_SendPaired(mac_dst);
            break;
      
        default:
            break;
        }
    }
    return event;
}


