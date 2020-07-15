
#ifndef __FUN_CONTROL__
#define __FUN_CONTROL__


#include "tl_common.h"

typedef enum
{
    CONTROL=0,
    BOUND,
    UNBOUND,
}fun_control_sm_t;

extern fun_control_sm_t fun_control_sm;

void fun_control_init(void);
void fun_control_sm_mgr(fun_control_sm_t sm_state);

#endif



