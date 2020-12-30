#include "tl_common.h"
#include "drivers.h"


#define PWM_W       GPIO_PC4		//white

#define PWM_FUNC_W  AS_PWM  // AS_PWM_SECOND

#define PWMID_W     (GET_PWMID(PWM_W, PWM_FUNC_W))
                    
#define PWM_INV_W   (GET_PWM_INVERT_VAL(PWM_W, PWM_FUNC_W))


#define LIGHT_CNT                       (1)     // means instance count


#if(CLOCK_SYS_CLOCK_HZ == 48000000)
#define PWM_CLK_DIV_LIGHT   (1)
#else
#define PWM_CLK_DIV_LIGHT   (0)
#endif

#ifndef PWM_FREQ
#define PWM_FREQ	        (600)   // unit: Hz
#endif
#define PWM_MAX_TICK        ((CLOCK_SYS_CLOCK_HZ / (PWM_CLK_DIV_LIGHT + 1)) / PWM_FREQ)




typedef struct{
    u32 gpio;
    u8 id;
    u8 invert;
    u8 func;
    u8 rsv[1];
}light_res_hw_t;



void light_pwm_init(void);
void light_dim_set_hw(int idx, int idx2, u16 val);












