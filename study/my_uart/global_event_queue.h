#ifndef __EVENT_QUEUE_H
#define __EVENT_QUEUE_H	 

#include "tl_common.h"
#include "drivers.h"


typedef enum
{
	EVENT_KEY1_PRESSED				= 0x00000001,
	EVENT_KEY2_PRESSED				= 0x00000002,
	EVENT_KEY1_LONG_PRESSED			= 0x00000004,
	EVENT_KEY2_LONG_PRESSED			= 0x00000008,
	EVENT_KEY1_AND_KEY2_PRESSED		= 0x00000010,
	EVENT_BOUND_SUCCESS				= 0x00000020,
	EVENT_BOUND_FAIL				= 0x00000040,
	EVENT_UNBOUND_SUCCESS			= 0x00000080,
	EVENT_UNBOUND_FAIL				= 0x00000100,
	
}event_type_t;


typedef event_type_t (*event_handle)(event_type_t event);

void Init_event_queue(void);
void resgister_event_handle(event_handle handle,int	interest_event_mask);
void event_establish(event_type_t event);

		 				    
#endif
