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
    resgister_event_handle(fun_boundevent_handle,EVENT_ONOFF_ONE|EVENT_PAIR_IN);
}


static inline void AndonPair_Wakeup(void)
{
    uint8_t pack_load;
    pack_load = ADV_PARILOAD_WAKEUP;
    adv_vendor_send_cmd(ADV_CMD_PARI,(uint8_t*)&pack_load,sizeof(pack_load),0);
}







int fun_bound_process(void)
{
   
    return 0;
}

void fun_bound_in(void)
{
    device_led_setup(led_cfg[REMOTE_LED_KEY_PRESS]);
    AndonPair_Wakeup();
}

void fun_bound_out(void)
{
    device_led_setup(led_cfg[REMOTE_LED_KEY_PRESS]);
 
}


static event_type_t fun_boundevent_handle(event_type_t event)
{
    if (apc_sm == BOUND)
    {
        switch (event)
        {
        case EVENT_ONOFF_ONE: //选灯
        
            break;
        case EVENT_PAIR_IN:
            apc_sm = CONTROL;
            fun_control_sm_mgr(apc_sm);
            break; 
      
        default:
            break;
        }
    }
    return event;
}


