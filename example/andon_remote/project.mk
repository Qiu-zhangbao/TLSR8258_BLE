################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 

OBJS += \
$(OUT_PATH)/main.o \
$(OUT_PATH)/app.o\
$(OUT_PATH)/global_event_queue.o\
$(OUT_PATH)/apc_fun_sm.o\
$(OUT_PATH)/fun_control.o\
$(OUT_PATH)/fun_bound.o\
$(OUT_PATH)/fun_unbound.o\
$(OUT_PATH)/AES.o\
$(OUT_PATH)/bsl_adv.o\
$(OUT_PATH)/hdl_key.o\
$(OUT_PATH)/hdl_uart.o\

#include "stack/ble/ble.h"
#include "tl_common.h"
#include "drivers.h"
#include "tinyFlash/tinyFlash.h"
#include "vendor/common/blt_led.h"
#include "vendor/common/blt_soft_timer.h"
#include "vendor/common/blt_common.h"

#include "global_event_queue.h"

#include "fun_bound.h"
#include "fun_control.h"
#include "fun_unbound.h"

#include "bsl_adv.h"

#include "hdl_key.h"
#include "hdl_light.h"
#include "hdl_uart.h"

# Each subdirectory must supply rules for building sources it contributes
$(OUT_PATH)/%.o: ./%.c
	@echo 'Building file: $<'
	@tc32-elf-gcc $(GCC_FLAGS) $(INCLUDE_PATHS) -c -o"$@" "$<"



