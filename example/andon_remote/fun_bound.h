
#ifndef __FUN_BOUND__
#define __FUN_BOUND__

#include "tl_common.h"
#include "drivers.h"

#define ADV_PARILOAD_DOPAIR       0x00
#define ADV_PARILOAD_PAIRACK      0x01
#define ADV_PARILOAD_PAIRED       0x02
#define ADV_PARILOAD_SELECT       0x03
#define ADV_PARILOAD_WAKEUP       0x07
#define ADV_PARILOAD_RESET        0xFF

#define ADV_CMD_PARI                0x0200


void fun_bound_init(void);
void fun_bound_in(void);
void fun_bound_out(void);

#endif



