
#include "fun_bound.h"
#include "fun_control.h"
#include "tl_common.h"
#include "drivers.h"
#include "global_event_queue.h"
#include "vendor/common/blt_soft_timer.h"
#include "vendor/common/blt_led.h"
#include "hdl_oled.h"
#include <stack/ble/ble.h>
#include "bsl_adv.h"
#if (DEVICE_TYPE == REMOTE)

static event_type_t fun_boundevent_handle(event_type_t event);
int fun_bound_process(void);
void fun_bound_add_mac_list(u8 *data,u8 len);

u8 bound_list[10][6]={0};
static u8 bound_cnt=0;
static u8 hextab[16] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};


void fun_bound_init(void)
{
    resgister_event_handle(fun_boundevent_handle,   EVENT_KEY1_SHORT_PRESSED |  EVENT_KEY2_SHORT_PRESSED | \
    EVENT_KEY1_LONG_PRESSED	 |  EVENT_KEY2_LONG_PRESSED  |  EVENT_KEY1_AND_KEY2_PRESSED );
    blt_soft_timer_add(fun_bound_process,100*1000);//100ms
}
void OLED_Show_Mac_List(u8 (*data)[6])
{
    u8 mac_list[10][12]={0};
    
    for(u8 i =0; i < bound_cnt; i ++)
	{
        for (u8 k = 0; k < 6; k++)
        {
            mac_list[i][k*2] = hextab[(data[i][k] >> 4)];
		    mac_list[i][k*2 +1] = hextab[(data[i][k]&0xf)];
        }
        if (i <= 3)
        {
            OLED_ShowString(10,2*i,&mac_list[i],16);
        }
	}



}

int fun_bound_process(void)
{
    OLED_Show_Mac_List(bound_list);
	return 0;
}

void fun_bound_in(void)
{
	// OLED_ShowString(30,0,"bound",16);
	device_led_setup(led_cfg[REMOTE_LED_BOUND_MODE]);//绑定模式开始闪烁
    bsl_adv_add_callback(fun_bound_add_mac_list);

}
void fun_bound_out(void)
{
	OLED_Clear();
	device_led_setup(led_cfg[REMOTE_LED_KEY_PRESS]);//取消闪烁
    memset(bound_list,0,sizeof(bound_list));
    bound_cnt=0;
}


void fun_bound_add_mac_list(u8 *data,u8 len)
{
    for (u8 i = 0; i < 10; i++)
    {
        if ((memcmp(&bound_list[i][0], data, 6) == 0))
            return;
       if (bound_list[i][0]==0)
       {
            memcpy(&bound_list[i][0], data, len);
            bound_cnt++;
            return;
       }
    }
 
}


static event_type_t fun_boundevent_handle(event_type_t event)
{
    if (fun_control_sm == BOUND)
    {
        switch(event)
        {
			case EVENT_KEY1_SHORT_PRESSED://选灯
				bsl_adv_led_all_bound(1);
                break;
			case EVENT_KEY1_LONG_PRESSED://确认选择
			
			break;
            case EVENT_KEY2_SHORT_PRESSED://退出绑定
				fun_control_sm = CONTROL;
                break;
            default:
                break;
        }
    }
    return event;
}



#endif





















