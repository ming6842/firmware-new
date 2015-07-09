TARGET_BOARD='Vertigo v2.0'

STARTUP_SRCS=$(WORKSPACE_DIR)/startup_stm32f427xx.c
EXTERNAL_DEVICE_SRCS = $(EXTERNAL_DEVICE)/AT24C04C.c \
			$(EXTERNAL_DEVICE)/mpu9250.c \
			$(EXTERNAL_DEVICE)/hmc5983.c \
			$(EXTERNAL_DEVICE)/lea6h_ubx.c \
			$(EXTERNAL_DEVICE)/ADS1246_MPX6115A.c

MCU_PERIPH_SRCS =  \
	$(MCU_PERIPH)/i2c.c \
	$(MCU_PERIPH)/spi.c \
	$(MCU_PERIPH)/gpio.c \
	$(MCU_PERIPH)/tim.c \
	$(MCU_PERIPH)/led.c \
	$(MCU_PERIPH)/usart.c \
	$(MCU_PERIPH)/input_capture.c \
	$(MCU_PERIPH)/system_time.c

ACTUATORS_SRCS = $(ACTUATORS)/pwm.c

RADIO_CONTROLLER_SRCS = $(RADIO_CONTROLLER)/radio_control.c \
	$(RADIO_CONTROLLER)/pwm_decoder.c

COMMON_SRCS =$(COMMON)/test_common.c \
	$(COMMON)/memory.c \
	$(COMMON)/io.c \
	$(COMMON)/std.c

CONTROLLER_SRCS = $(CONTROLLER)/attitude_stabilizer.c \
		$(CONTROLLER)/vertical_stabilizer.c \
		$(CONTROLLER)/navigation.c \
		$(CONTROLLER)/flight_controller.c \
		$(CONTROLLER)/controller.c

FREERTOS_SRCS=$(FREERTOS)/Source/croutine.c \
	$(FREERTOS)/Source/list.c \
	$(FREERTOS)/Source/queue.c \
	$(FREERTOS)/Source/tasks.c \
	$(FREERTOS)/Source/timers.c \
	$(FREERTOS)/Source/portable/MemMang/heap_1.c \
	$(FREERTOS)/Source/portable/GCC/ARM_CM4F/port.c

MAVLINK_SRCS=$(MAVLINK)/communication.c \
	$(MAVLINK)/mission.c \
	$(MAVLINK)/parameter.c \
	$(MAVLINK)/command_parser.c \
	$(MAVLINK)/global.c

CMSIS_SRCS= \
	$(CMSIS)/FastMathFunctions/arm_cos_f32.c \
	$(CMSIS)/FastMathFunctions/arm_sin_f32.c

STMF4_STD_DRIVER_SRCS= \
	$(WORKSPACE_DIR)/system_stm32f4xx.c \
        $(ST)/src/misc.c \
        $(ST)/src/stm32f4xx_rcc.c \
        $(ST)/src/stm32f4xx_dma.c \
        $(ST)/src/stm32f4xx_flash.c \
        $(ST)/src/stm32f4xx_gpio.c \
        $(ST)/src/stm32f4xx_usart.c \
        $(ST)/src/stm32f4xx_tim.c \
        $(ST)/src/stm32f4xx_spi.c \
        $(ST)/src/stm32f4xx_i2c.c \
        $(ST)/src/stm32f4xx_sdio.c

BASIC_SRCS= \
        $(WORKSPACE_DIR)/interrupt.c \
        $(WORKSPACE_DIR)/main.c \


#
#generate the source file list
#
SRCS += $(STARTUP_SRCS)
SRCS += $(EXTERNAL_DEVICE_SRCS)
SRCS += $(MCU_PERIPH_SRCS)
SRCS += $(ACTUATORS_SRCS)
SRCS += $(RADIO_CONTROLLER_SRCS)
SRCS += $(COMMON_SRCS)
SRCS += $(CONTROLLER_SRCS)
SRCS += $(FREERTOS_SRCS)
SRCS += $(MAVLINK_SRCS)
SRCS += $(CMSIS_SRCS)
SRCS += $(STMF4_STD_DRIVER_SRCS)
SRCS += $(BASIC_SRCS)
