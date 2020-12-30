

#include "tl_common.h"
#include "drivers.h"
#include "hdl_key.h"
#include "global_event_queue.h"
#include "app_config.h"
#include "hdl_light.h"


#define RES_HW_PWM_W    {PWM_W, PWMID_W, PWM_INV_W, PWM_FUNC_W}


const light_res_hw_t light_res_hw[LIGHT_CNT][1] = {
	/*[0] = */{RES_HW_PWM_W}
};


void light_pwm_init(void)
{
    foreach(i, LIGHT_CNT){
    	foreach_arr(k,light_res_hw[0]){
			u16 level_def = 0;	// PWM_MAX_TICK;	 //
			const light_res_hw_t *p_hw = &light_res_hw[i][k];
	        pwm_set_cycle_and_duty(p_hw->id, PWM_MAX_TICK, p_hw->invert ? (PWM_MAX_TICK - level_def) : level_def);
	        pwm_start(p_hw->id);
	        gpio_set_func(p_hw->gpio, p_hw->func);
	        
        }
    }
}



void pwm_set_lum (int id, u16 y, int pol)
{

    u32 level = ((u32)y * PWM_MAX_TICK) / (255*256);

	pwm_set_cmp (id, pol ? PWM_MAX_TICK - level : level);
}



/**
 * @brief  Control lights on hardware.
 * @param  idx: 1st index of light_res_hw array, also means index 
 *   of LIGHT_CNT.
 * @param  idx2: The 2nd index of light_res_hw array.
 * @param  val: Value.
 * @retval None
 */
void light_dim_set_hw(int idx, int idx2, u16 val)
{
    if((idx < ARRAY_SIZE(light_res_hw)) && (idx2 < ARRAY_SIZE(light_res_hw[0]))){
		const light_res_hw_t *p_hw = &light_res_hw[idx][idx2];
		#if FEATURE_LOWPOWER_EN
		led_onoff_gpio(p_hw->gpio, 0 != val);
		#else
        pwm_set_lum(p_hw->id, val, p_hw->invert);
        #endif
    }
}









