#include "apc_fun_sm.h"
#include "global_event_queue.h"
#include "fun_control.h"
#include "fun_unbound.h"
#include "fun_bound.h"
#include "vendor/common/blt_soft_timer.h"
#include "vendor/common/blt_led.h"

apc_sm_t apc_sm = CONTROL;
apc_sm_t apc_sm_last = STANDBY;

led_cfg_t led_cfg[] = {
		{10,     10,      1,   	  1,  	},// REMOTE_LED_KEY_PRESS,
		{500,	  500 ,	  2,	  0,    },// REMOTE_LED_BOUND_SUCCESS,
		{100,	  100 ,   2,	  0,    },// REMOTE_LED_BOUND_FAIL,
		{500,	  500 ,   2,	  0,	 },// REMOTE_LED_UNBOUND_SUCCESS,
		{100,	  100 ,   2,	  0, 	 },// REMOTE_LED_UNBOUND_FAIL,
		{1000,	  1000 ,  180,	  0, 	 },// REMOTE_LED_BOUND_MODE,
		{100,	  1000 ,  180,	  0, 	 },// REMOTE_LED_UNBOUND_MODE,
		{1000,	  1000 ,  3,	  0,	 },// LIGHT_LED_RECOVER,	
};


int apc_fun_sm_process(void);
void fun_control_sm_mgr(apc_sm_t sm_state);


//static event_type_t apc_fun_sm_handle(event_type_t event);

void apc_fun_sm_init(void)
{
   // resgister_event_handle(apc_fun_sm_handle, EVENT_ONOFF_ONE);
    fun_control_init();
    fun_bound_init();
	fun_unbound_init();
	blt_soft_timer_add(apc_fun_sm_process,100*1000);//100ms
}


int apc_fun_sm_process(void)
{
    //fun_control_sm_mgr(apc_sm);
	return 0;
}

void fun_control_sm_mgr(apc_sm_t sm_state)
{
	if(sm_state == apc_sm_last)
		return;
	switch(apc_sm_last) 
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
	
	apc_sm_last = sm_state;
}


// static event_type_t apc_fun_sm_handle(event_type_t event)
// {
//         switch (event)
//         {
//         case EVENT_ONOFF_ONE: //选灯
           
//             break;
//         default:
//             break;
//         }
    
//     return event;
// }


