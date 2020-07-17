
#include "fun_unbound.h"
#include "fun_control.h"
#include "tl_common.h"
#include "drivers.h"
#include "global_event_queue.h"
#include "vendor/common/blt_soft_timer.h"
#include "vendor/common/blt_led.h"
#include "hdl_oled.h"
#include <stack/ble/ble.h>

#if (DEVICE_TYPE == REMOTE)

static event_type_t fun_unboundevent_handle(event_type_t event);
int fun_unbound_process(void);


void fun_unbound_init(void)
{
    resgister_event_handle(fun_unboundevent_handle,   EVENT_KEY1_SHORT_PRESSED |  EVENT_KEY2_SHORT_PRESSED | \
    EVENT_KEY1_LONG_PRESSED	 |  EVENT_KEY2_LONG_PRESSED  |  EVENT_KEY1_AND_KEY2_PRESSED );
    blt_soft_timer_add(fun_unbound_process,100*1000);//100ms
}


int fun_unbound_process(void)
{
    
	
	return 0;
}

void fun_unbound_in(void)
{
	OLED_ShowString(30,0,"unbound",16);
    device_led_setup(led_cfg[REMOTE_LED_UNBOUND_MODE]);
    bsl_adv_led_all_bound(0);
}
void fun_unbound_out(void)
{
	OLED_Clear();
    device_led_setup(led_cfg[REMOTE_LED_KEY_PRESS]);//取消闪烁

}


static event_type_t fun_unboundevent_handle(event_type_t event)
{
    if (fun_control_sm == UNBOUND)
    {
        switch(event)
        {
			case EVENT_KEY1_SHORT_PRESSED://选灯
                
                break;
			case EVENT_KEY1_LONG_PRESSED://确认选择
			
			break;
            case EVENT_KEY2_SHORT_PRESSED://退出解绑
				fun_control_sm = CONTROL;
                break;
            default:
                break;
        }
    }
    return event;
}





#endif



















