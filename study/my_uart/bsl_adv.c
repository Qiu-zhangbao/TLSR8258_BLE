#include "bsl_adv.h"
#include "fun_control.h"
#include "tl_common.h"
#include "drivers.h"
#include "global_event_queue.h"
#include "vendor/common/blt_soft_timer.h"
#include "vendor/common/blt_led.h"
#include <stack/ble/ble.h>




void bsl_adv_init(void)
{
#if (DEVICE_TYPE == REMOTE)



#elif(DEVICE_TYPE == LIGHT)




#endif


}

#if (DEVICE_TYPE == REMOTE)

void bsl_adv_led_onoff(u8 on)
{
	if (on)
	{
		u8 tbl_advData[] = { 0x05, 0x09, 'A', 'B', 'C', 'D'}; //要广播的数据
		bls_ll_setAdvData( (u8 *)tbl_advData, sizeof(tbl_advData) ); //设置广播数据
	}
	else
	{
		u8 tbl_advData[] = { 0x05, 0x09, 'A', 'B', 'C', 'C'}; //要广播的数据
		bls_ll_setAdvData( (u8 *)tbl_advData, sizeof(tbl_advData) ); //设置广播数据
	}
}

#elif(DEVICE_TYPE == LIGHT)




#endif


















