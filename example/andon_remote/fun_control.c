#include "fun_control.h"
#include "global_event_queue.h"
#include "apc_fun_sm.h"
#include "vendor/common/blt_soft_timer.h"
#include "vendor/common/blt_led.h"
#include "app_config.h"
#include "bsl_adv.h"

static event_type_t fun_controlevent_handle(event_type_t event);
int fun_control_process(void);

extern int qedc_value;

void fun_control_init(void)
{
    resgister_event_handle(fun_controlevent_handle, EVENT_ONOFF_ONE|EVENT_ONOFF_LONG|EVENT_PAIR_OUT|EVENT_PAIR_IN);
  
    blt_soft_timer_add(fun_control_process,10*1000);//100ms
}

int fun_control_process(void)
{
    static int qedc_value_old=0;
    if ( qedc_value_old != qedc_value)
    {
        qedc_value_old = qedc_value;
        
    }
    
    
    
    
	return 0;
}

void fun_control_in(void)
{


}
void fun_control_out(void)
{
	
}

static event_type_t fun_controlevent_handle(event_type_t event)
{
    if (apc_sm == CONTROL)
    {
        switch(event)
        {
			case EVENT_ONOFF_ONE:
                device_led_setup(led_cfg[REMOTE_LED_KEY_PRESS]); //开灯
                break;
            case EVENT_ONOFF_LONG:
                device_led_setup(led_cfg[REMOTE_LED_BOUND_MODE]); //开灯
                break;
            case EVENT_PAIR_OUT:
                apc_sm = BOUND;
                fun_control_sm_mgr(apc_sm);
                break; 
           
            default:
                break;
        }
    }
    return event;
}






















