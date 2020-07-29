
#include "fun_control.h"
#include "tl_common.h"
#include "drivers.h"
#include "global_event_queue.h"
#include "vendor/common/blt_soft_timer.h"
#include "vendor/common/blt_led.h"
#include "hdl_oled.h"
#include <stack/ble/ble.h>

#include "fun_bound.h"
#include "fun_unbound.h"
#include "bsl_adv.h"

#if (DEVICE_TYPE == REMOTE)

static event_type_t fun_controlevent_handle(event_type_t event);
int fun_control_process(void);
fun_control_sm_t fun_control_sm = CONTROL;
fun_control_sm_t fun_control_sm_last = STANDBY;

fun_control_sm_t fun_control_sm_now = CONTROL;


void fun_control_init(void)
{
    resgister_event_handle(fun_controlevent_handle,   EVENT_KEY1_SHORT_PRESSED |  EVENT_KEY2_SHORT_PRESSED | \
    EVENT_KEY1_LONG_PRESSED	 |  EVENT_KEY2_LONG_PRESSED  |  EVENT_KEY1_AND_KEY2_PRESSED | EVENT_KEY1_AND_KEY2_PRESSED);
    blt_soft_timer_add(fun_control_process,100*1000);//100ms

	fun_bound_init();
	fun_unbound_init();
}


int fun_control_process(void)
{
    fun_control_sm_mgr(fun_control_sm);
	return 0;
}

void fun_control_in(void)
{
	OLED_ShowString(30,0,"control",16);
	OLED_ShowString(30,4,"state:",16);
	fun_oled_show_state(0);
	fun_control_sm_now = CONTROL;
	bsl_adv_remote_state(fun_control_sm_now);

}
void fun_control_out(void)
{
	OLED_Clear();
}

void fun_control_sm_mgr(fun_control_sm_t sm_state)
{
	if(sm_state == fun_control_sm_last)
		return;
	switch(fun_control_sm_last) 
	{
		case CONTROL:	
        	fun_control_out();
            break;                          
		case BOUND:         
			fun_bound_out();
			break;                          
		case UNBOUND:           
			fun_unbound_out();
			break;  
		case STANDBY:   
			
            break;                          
	
	}
	switch(sm_state)
	{
		case CONTROL:	
            fun_control_in();
			break;
		case BOUND:
			fun_bound_in();
			break;
		case UNBOUND:	
            fun_unbound_in();
			break;
		case STANDBY:   

            break;
	
	}
	
	fun_control_sm_last = sm_state;
}


void fun_oled_show_state(u8 on)
{
	if (on)
	OLED_ShowString(80,4,"off",16);
	else
	OLED_ShowString(80,4,"on ",16);
	
}


static event_type_t fun_controlevent_handle(event_type_t event)
{
    if (fun_control_sm_now == CONTROL)
    {
        switch(event)
        {
			case EVENT_KEY2_SHORT_PRESSED://开关键
                device_led_setup(led_cfg[REMOTE_LED_KEY_PRESS]);
                bsl_adv_led_onoff(1);
				fun_oled_show_state(1);
                break;
            case EVENT_KEY1_SHORT_PRESSED://开关键
                device_led_setup(led_cfg[REMOTE_LED_KEY_PRESS]);
                bsl_adv_led_onoff(0);
				fun_oled_show_state(0);
                break;
            case EVENT_KEY1_LONG_PRESSED://进入绑定状态
                fun_control_sm = BOUND;
                break;
            case EVENT_KEY2_LONG_PRESSED://进入解绑状态
                fun_control_sm = UNBOUND; 
                break;
			case EVENT_KEY1_AND_KEY2_PRESSED://进入解绑状态
                OLED_ShowNum(80,4,2,1,16);
                break;
            default:
                break;
        }
    }
    return event;
}



#endif





















