################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 

OBJS += \
$(OUT_PATH)/main.o \
$(OUT_PATH)/app_uart.o\
$(OUT_PATH)/app.o\
$(OUT_PATH)/hdl_oled.o\
$(OUT_PATH)/hdl_key.o\
$(OUT_PATH)/global_event_queue.o\
$(OUT_PATH)/fun_control.o\
$(OUT_PATH)/fun_bound.o\
$(OUT_PATH)/fun_unbound.o\
$(OUT_PATH)/AES.o\
$(OUT_PATH)/bsl_adv.o\


# Each subdirectory must supply rules for building sources it contributes
$(OUT_PATH)/%.o: ./%.c
	@echo 'Building file: $<'
	@tc32-elf-gcc $(GCC_FLAGS) $(INCLUDE_PATHS) -c -o"$@" "$<"



