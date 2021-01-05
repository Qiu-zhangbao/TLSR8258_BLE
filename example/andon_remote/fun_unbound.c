
#include "fun_unbound.h"
#include "global_event_queue.h"
#include "apc_fun_sm.h"

static event_type_t fun_unboundevent_handle(event_type_t event);

void fun_unbound_init(void)
{
    resgister_event_handle(fun_unboundevent_handle,EVENT_ONOFF_ONE);

}

void fun_unbound_in(void)
{
 
}
void fun_unbound_out(void)
{
 
}

static event_type_t fun_unboundevent_handle(event_type_t event)
{
    if (0)
    {
        switch (event)
        {
        case EVENT_ONOFF_ONE: //选灯
          
            break;
       
        default:
            break;
        }
    }
    return event;
}

