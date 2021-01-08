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
static int delta_all = 0;

void fun_control_init(void)
{
    resgister_event_handle(fun_controlevent_handle, EVENT_ONOFF_ONE|EVENT_ONOFF_LONG|EVENT_PAIR_OUT|EVENT_PAIR_IN);
  
    blt_soft_timer_add(fun_control_process,10*1000);//100ms
}

//*****************************************************************************
// 函数名称: 
// 函数描述: 
// 函数输入:  
// 函数返回值: 
//*****************************************************************************/
void AndonCmd_Delta(int delta, uint16_t tran_time)
{
    uint8_t pack_load[8];
    uint8_t len = 0;

    if(delta > 100)
        delta = 100;
    else if(delta < -100)
        delta = -100;
    
    if(delta_all < -100 && delta > 0)
    {
        delta_all = 0;
    }

    if(delta_all > 100 && delta < 0)
    {
        delta_all = 0;
    }

    delta_all += delta;
    if(delta_all > 200)   //变化量超过变化范围的2倍，不在发送数据
    {
        delta_all = 200;
        delta = 100;
        return;
    } 
    else if(delta_all < -200) //变化量超过变化范围的2倍，不在发送数据
    {
        delta_all = -200;
        delta = -100;
        return;
    }
    
    pack_load[len++] = delta;

    if(tran_time > 62000)
        tran_time = 62000;
    if(tran_time > 6200)
    {
        tran_time = tran_time/1000;
        tran_time |= 0x40;
    }
    else
    {
        tran_time = tran_time/100;
    }
    pack_load[len++] = tran_time&0xFF;
   
    adv_vendor_send_cmd(ADV_CMD_DELTABRIGHTNESS,pack_load,len,ADV_PACK_TTL);
    
}

int fun_control_process(void)
{
    static int qedc_value_old=0;
    static int delta_light=0;

    if (( qedc_value_old != qedc_value) && ((qedc_value - qedc_value_old)%4==0))
    {
        delta_light= (qedc_value - qedc_value_old)/4;
        
        AndonCmd_Delta(delta_light,500);
        device_led_setup(led_cfg[REMOTE_LED_KEY_PRESS]); //开灯

        qedc_value_old = qedc_value;
    }
    
	return 0;
}
//*****************************************************************************
// 函数名称: 
// 函数描述: 
// 函数输入:  delay 延迟时间单位ms，超过62s时，设置为0xFF
// 函数返回值: 
//*****************************************************************************/
void AndonCmd_Toggle(uint16_t delay)
{
    uint8_t pack_load[8];

    #if(ANDONCMD_TRANS_TIME > 62000)
        pack_load[0] = 62000/1000;
        pack_load[0] |= 0x40;
    #elif(ANDONCMD_TRANS_TIME > 6200)   
        pack_load[0] = ANDONCMD_TRANS_TIME/1000;
        pack_load[0] |= 0x40;
    #else
        pack_load[0] = ANDONCMD_TRANS_TIME/100;
    #endif
    
    
    if(delay > 62000)
    {
        pack_load[1] = 0xFF;
    }    
    else
    {
        if(delay > 6200)
        {
            delay = delay/1000;
            delay |= 0x40;
        }
        else
        {
            delay = delay/100;
        }
    }
    pack_load[1] = delay;

    // if(mesh_app_node_is_provisioned())
    // {
    //     mesh_vendor_send_cmd(VENDOR_SETTOGGLEONOFF_CMD,pack_load,2);
    //     //adv_vendor_send_cmd(ADV_CMD_SETTOGGLEONOFF_CMD,pack_load,2,ADV_PACK_TTL);
    // }
    // else
    // {
        adv_vendor_send_cmd(ADV_CMD_SETTOGGLEONOFF_CMD,pack_load,2,ADV_PACK_TTL);
    // }
    
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
                AndonCmd_Toggle(0);
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






















