
#include "fun_bound.h"
#include "tl_common.h"
#include "drivers.h"
#include "global_event_queue.h"
#include "vendor/common/blt_soft_timer.h"
#include "vendor/common/blt_led.h"
#include "hdl_oled.h"
#include <stack/ble/ble.h>
static event_type_t Fun_CtrlSMEventHandle(event_type_t event);
int fun_bound_process(void);
fun_bound_sm_t fun_bound_sm = CONTROL;
fun_bound_sm_t fun_bound_sm_last = UNBOUND;
u8 key2_press_cnt=0;

void fun_bound_init(void)
{
    resgister_event_handle(Fun_CtrlSMEventHandle,   EVENT_KEY1_SHORT_PRESSED |  EVENT_KEY2_SHORT_PRESSED | \
    EVENT_KEY1_LONG_PRESSED	 |  EVENT_KEY2_LONG_PRESSED  |  EVENT_KEY1_AND_KEY2_PRESSED );
    blt_soft_timer_add(fun_bound_process,100*1000);//100ms
}


int fun_bound_process(void)
{
    fun_bound_sm_mgr(fun_bound_sm);
	return 0;
}




void fun_bound_sm_mgr(fun_bound_sm_t sm_state)
{
	if(sm_state == fun_bound_sm_last)
		return;
	switch(fun_bound_sm_last) 
	{
		case CONTROL:	
        	OLED_Clear();
            break;                          
		case BOUND:         
			OLED_Clear();
			break;                          
		case UNBOUND:           
			OLED_Clear();
			break;                            
	
	}
	switch(sm_state)
	{
		case CONTROL:	
            OLED_ShowString(30,0,"bound",16);
            OLED_ShowString(30,4,"state:",16);
            OLED_ShowNum(80,4,0,1,16);
			break;
		case BOUND:
			OLED_ShowString(30,0,"bound",16);
			break;
		case UNBOUND:	
            OLED_ShowString(30,0,"unbound",16);
			break;
	
	}
	
	fun_bound_sm_last = sm_state;
}

void bsl_led_onoff(u8 on)
{
	if (on)
	{
		u8 tbl_advData[] = { 0x05, 0x09, 'A', 'B', 'C', 'D'}; //要广播的数据
		bls_ll_setAdvData( (u8 *)tbl_advData, sizeof(tbl_advData) ); //设置广播数据
        OLED_ShowNum(80,4,0,1,16);
	}
	else
	{
		u8 tbl_advData[] = { 0x05, 0x09, 'A', 'B', 'C', 'C'}; //要广播的数据
		bls_ll_setAdvData( (u8 *)tbl_advData, sizeof(tbl_advData) ); //设置广播数据
        OLED_ShowNum(80,4,1,1,16);
	}
}


static event_type_t Fun_CtrlSMEventHandle(event_type_t event)
{
    if (fun_bound_sm == CONTROL)
    {
        switch(event)
        {
            case EVENT_KEY2_SHORT_PRESSED:
                device_led_setup(led_cfg[REMOTE_LED_KEY_PRESS]);
                key2_press_cnt++;
                bsl_led_onoff((key2_press_cnt-1)%2);
                break;
            case EVENT_KEY1_LONG_PRESSED://进入绑定状态
                fun_bound_sm = BOUND;
                break;
            case EVENT_KEY2_LONG_PRESSED://进入解绑状态
                fun_bound_sm = UNBOUND; 
                break;
            default:
                break;
        }
    }
    return event;
}

























