
#ifndef __Apc_FunCtrlSM__
#define __Apc_FunCtrlSM__

#include "tl_common.h"
#include "drivers.h"
#include "vendor/common/blt_led.h"

typedef enum
{
    CONTROL=0,
    BOUND,
    UNBOUND,
    STANDBY,
}apc_sm_t;
extern apc_sm_t apc_sm;

void apc_fun_sm_init(void);
void fun_control_sm_mgr(apc_sm_t sm_state);

enum{
	REMOTE_LED_KEY_PRESS,
	REMOTE_LED_BOUND_SUCCESS,
	REMOTE_LED_BOUND_FAIL,
	REMOTE_LED_UNBOUND_SUCCESS,
	REMOTE_LED_UNBOUND_FAIL,
	REMOTE_LED_BOUND_MODE,
	REMOTE_LED_UNBOUND_MODE,
    ON,
    OFF
};



enum
{
    STORAGE_BOUND_MAC = 1,
	STORAGE_RESET_CNT,
	STORAGE_LIGHT_STATE,
};

extern led_cfg_t led_cfg[];

#endif



