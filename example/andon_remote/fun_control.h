
#ifndef __FUN_CONTROL__
#define __FUN_CONTROL__


#include "tl_common.h"
#include "drivers.h"


#define ANDONCMD_TRANS_TIME            850    //单位为ms









#define ADV_CMD_DELTABRIGHTNESS    0x0302
#define ADV_CMD_SETONOFF_CMD       0x0201
#define ADV_CMD_SETTOGGLEONOFF_CMD  0x0401



void fun_control_init(void);
void fun_control_in(void);
void fun_control_out(void);


#endif



