
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
void fun_bound_add_mac_list(u8 (*data)[6], u8 len);

u8 bound_list[10][6];
static u8 bound_cnt = 0;
static u8 select_now=0;
static char hextab[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

void fun_bound_init(void)
{
    resgister_event_handle(fun_boundevent_handle, EVENT_KEY1_SHORT_PRESSED | EVENT_KEY2_SHORT_PRESSED |
                                                      EVENT_KEY1_LONG_PRESSED | EVENT_KEY2_LONG_PRESSED | EVENT_KEY1_AND_KEY2_PRESSED |\
                                                      EVENT_KEY1_AND_KEY2_PRESSED);
    bsl_adv_add_callback(fun_bound_add_mac_list);

}
void OLED_Show_Mac_List(u8 (*data)[6])
{
    char mac_list[10][12]={0};
    for (u8 i = 0; i < bound_cnt; i++)
    {
        for (u8 k = 0; k < 6; k++)
        {
            mac_list[i][k * 2] = hextab[(data[i][k] >> 4)];
            mac_list[i][k * 2 + 1] = hextab[(data[i][k] & 0xf)];
        }
        if (bound_cnt<= 4)
        {
            OLED_ShowString(10, 2 * i, &mac_list[i][0], 16);
        }
    }
}
int fun_led_light_one(void)
{
    if(bound_cnt>0)
    bsl_adv_one_bound(&bound_list[0],1,0);
    return -1;
}
int fun_bound_process(void)
{
    static u8 num_cnt = 0;
    static u16 time = 0;
    time++;
    if (time>1800)//大于三分钟自动退出
    {
        fun_control_sm = CONTROL;
        time=0;
    }

    
    if (bound_cnt > 0) //有设备，显示地址
    {
        OLED_Show_Mac_List(bound_list);
    }
    else
    {
        num_cnt++;
        if (num_cnt > 10)
        {
            OLED_ShowString(16, 0, "no new device", 16);
            num_cnt = 0;
        }
    }
    return 0;
}

void fun_bound_in(void)
{
    device_led_setup(led_cfg[REMOTE_LED_BOUND_MODE]); //绑定模式开始闪烁
    blt_soft_timer_add(fun_bound_process, 100 * 1000); //100ms
    blt_soft_timer_add(fun_led_light_one, 2000 * 1000);
    select_now=0;
    fun_control_sm_now = BOUND;
    bsl_adv_remote_state(fun_control_sm_now);//查询绑定
    bls_ll_setAdvEnable(1); 
}

void fun_bound_out(void)
{
    OLED_Clear();
    device_led_setup(led_cfg[REMOTE_LED_KEY_PRESS]); //取消闪烁
    blt_soft_timer_delete(fun_bound_process);
    memset(bound_list, 0, sizeof(bound_list));
    bound_cnt = 0;
    bls_ll_setAdvEnable(0); 
}

void fun_bound_add_mac_list(u8 (*data)[6], u8 len)
{
    for (u8 i = 0; i < 10; i++)
    {
        if ((memcmp(&bound_list[i][0], data, 6) == 0))
            return;
        if (bound_list[i][0] == 0)
        {
            memcpy(&bound_list[i][0], data, len);
            bound_cnt++;
            return;
        }
    }
}

static event_type_t fun_boundevent_handle(event_type_t event)
{
    if ( fun_control_sm_now == BOUND)
    {
        switch (event)
        {
        case EVENT_KEY1_SHORT_PRESSED: //选灯
            select_now++;
            if(select_now>=bound_cnt)
                select_now=0;
            bsl_adv_one_bound(&bound_list[select_now],1,0);
            break;
        case EVENT_KEY1_LONG_PRESSED: //确认选择
            bsl_adv_one_bound(&bound_list[select_now],1,1);
            break;
        case EVENT_KEY2_SHORT_PRESSED: //退出绑定
            fun_control_sm = CONTROL;
            break;
        case EVENT_KEY1_AND_KEY2_PRESSED://进入解绑状态
            bsl_adv_led_all_bound(1);
            break;
        default:
            break;
        }
    }
    return event;
}

#endif
