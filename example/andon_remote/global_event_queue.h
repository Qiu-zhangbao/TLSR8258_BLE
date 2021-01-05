#ifndef __EVENT_QUEUE_H
#define __EVENT_QUEUE_H	 
#include "tl_common.h"
#include "drivers.h"

typedef enum
{
	EVENT_ONOFF_ONE		= 0x00000001,
	EVENT_ONOFF_DOUBLE	= 0x00000002,
	EVENT_ONOFF_LONG	= 0x00000004,
	EVENT_RESET_LONG	= 0x00000008,
	EVENT_PAIR_IN		= 0x00000010,
	EVENT_PAIR_OUT		= 0x00000020,
	
}event_type_t;


typedef event_type_t (*event_handle)(event_type_t event);

void Init_event_queue(void);
void resgister_event_handle(event_handle handle,int	interest_event_mask);
void event_establish(event_type_t event);

		 				    
#endif
