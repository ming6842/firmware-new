TARGET_BOARD='Vertigo v2.0'

STARTUP=$(WORKSPACE_DIR)/startup_stm32f427xx.o
EXTERNAL_DEVICE_SRC = $(EXTERNAL_DEVICE)/AT24C04C.o \
			$(EXTERNAL_DEVICE)/mpu9250.o \
			$(EXTERNAL_DEVICE)/hmc5983.o \
			$(EXTERNAL_DEVICE)/lea6h_ubx.o \
			$(EXTERNAL_DEVICE)/ADS1246_MPX6115A.o

MCU_PERIPH_SRC =  \
	$(MCU_PERIPH)/i2c.o \
	$(MCU_PERIPH)/spi.o \
	$(MCU_PERIPH)/gpio.o \
	$(MCU_PERIPH)/tim.o \
	$(MCU_PERIPH)/led.o \
	$(MCU_PERIPH)/usart.o \
	$(MCU_PERIPH)/input_capture.o \
	$(MCU_PERIPH)/system_time.o

ACTUATORS_SRC = $(ACTUATORS)/pwm.o

RADIO_CONTROLLER_SRC = $(RADIO_CONTROLLER)/radio_control.o \
	$(RADIO_CONTROLLER)/pwm_decoder.o

COMMON_SRC =$(COMMON)/test_common.o \
	$(COMMON)/memory.o \
	$(COMMON)/io.o \
	$(COMMON)/std.o

CONTROLLER_SRC = $(CONTROLLER)/attitude_stabilizer.o \
		$(CONTROLLER)/vertical_stabilizer.o \
		$(CONTROLLER)/navigation.o \
		$(CONTROLLER)/flight_controller.o \
		$(CONTROLLER)/controller.o

FREERTOS_SRC=$(FREERTOS)/Source/croutine.o \
	$(FREERTOS)/Source/list.o \
	$(FREERTOS)/Source/queue.o \
	$(FREERTOS)/Source/tasks.o \
	$(FREERTOS)/Source/timers.o \
	$(FREERTOS)/Source/portable/MemMang/heap_1.o \
	$(FREERTOS)/Source/portable/GCC/ARM_CM4F/port.o

MAVLINK_SRC=$(MAVLINK)/communication.o \
	$(MAVLINK)/mission.o \
	$(MAVLINK)/parameter.o \
	$(MAVLINK)/command_parser.o \
	$(MAVLINK)/global.o

OBJS=	$(WORKSPACE_DIR)/system_stm32f4xx.o \
	$(CMSIS)/FastMathFunctions/arm_cos_f32.o \
	$(CMSIS)/FastMathFunctions/arm_sin_f32.o \
        $(ST)/src/misc.o \
        $(ST)/src/stm32f4xx_rcc.o \
        $(ST)/src/stm32f4xx_dma.o \
        $(ST)/src/stm32f4xx_flash.o \
        $(ST)/src/stm32f4xx_gpio.o \
        $(ST)/src/stm32f4xx_usart.o \
        $(ST)/src/stm32f4xx_tim.o \
        $(ST)/src/stm32f4xx_spi.o \
        $(ST)/src/stm32f4xx_i2c.o \
        $(ST)/src/stm32f4xx_sdio.o \
        $(WORKSPACE_DIR)/interrupt.o \
        $(WORKSPACE_DIR)/main.o \
        $(STARTUP) \
        $(EXTERNAL_DEVICE_SRC) $(MCU_PERIPH_SRC) \
	$(ESTIMATOR_SRC) $(ACTUATORS_SRC) $(RADIO_CONTROLLER_SRC) \
	$(COMMON_SRC) $(CONTROLLER_SRC) $(FREERTOS_SRC) \
	$(MAVLINK_SRC)