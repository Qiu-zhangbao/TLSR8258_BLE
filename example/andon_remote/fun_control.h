
#ifndef __FUN_CONTROL__
#define __FUN_CONTROL__


#include "tl_common.h"
#include "drivers.h"


#define ANDONCMD_TRANS_TIME            850    //单位为ms









#define ADV_CMD_DELTABRIGHTNESS    0x0302
#define ADV_CMD_SETONOFF_CMD       0x0201
#define ADV_CMD_SETTOGGLEONOFF_CMD  0x0401

#define ADV_CMD_TOOLING           0xFF01
#define ADV_TOOLING_BURNIN        0x01    //无线功能测试 检测接收信号强度并配合检测灯的性能
#define ADV_TOOLING_AFTERBURN     0x02    
#define ADV_TOOLING_POWER_TEST    0x03    //测试最大亮度和最低亮度灯的功率
#define ADV_TOOLING_UPGRADE_VER   0x04    //批量升级版本

#define ADV_TOOLING_RSST_LIMIT          -80

void fun_control_init(void);
void fun_control_in(void);
void fun_control_out(void);


#endif



