
#ifndef __FUN_CONTROL__
#define __FUN_CONTROL__


#include "tl_common.h"

#if (DEVICE_TYPE == REMOTE)

typedef enum
{
    CONTROL=0,
    BOUND,
    UNBOUND,
    STANDBY,
}fun_control_sm_t;

extern fun_control_sm_t fun_control_sm;
extern fun_control_sm_t fun_control_sm_now;
void fun_control_init(void);
void fun_control_sm_mgr(fun_control_sm_t sm_state);

#endif
#endif



