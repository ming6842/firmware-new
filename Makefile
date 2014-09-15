#Output files
PROJECT=firmware
EXECUTABLE=$(PROJECT).elf
BIN_IMAGE=$(PROJECT).bin

export WORKSPACE_DIR=./program

#
#include .mk
include $(WORKSPACE_DIR)/makefiles/toolchain.mk
include $(WORKSPACE_DIR)/makefiles/workspace.mk
include $(WORKSPACE_DIR)/makefiles/rules.mk
#
#object file dir
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



#
#make target
all:$(BIN_IMAGE)

$(BIN_IMAGE):$(EXECUTABLE)
	@$(OBJCOPY) -O binary $^ $@
	@echo 'OBJCOPY $(BIN_IMAGE)'

$(EXECUTABLE): $(OBJS)
	@$(LD) $(LDFLAGS) $(ARCH_FLAGS) $(OBJS) $(LDLIBS) -o $@
	@echo 'LD $(EXECUTABLE)'

clean:
	rm -rf $(STARTUP_OBJ)
	rm -rf $(EXECUTABLE)
	rm -rf $(BIN_IMAGE)
	rm -f $(OBJS)

#
#upload firmware through st-flash
flash:
	st-flash write $(BIN_IMAGE) 0x8000000

#
#create gdb server through openocd
openocd: flash
	openocd -f ../debug/openocd.cfg

#
#execute cgdb 
cgdb:
	cgdb -d $(GDB) -x ./st_util_init.gdb

#
#execute gdbtui
gdbtui:
	$(GDB) -tui -x ./st_util_init.gdb

#
#upload firmware through black magic probe
flash_bmp:
	$(GDB) firmware.elf -x ./gdb_black_magic.gdb
#
#execute and connect to black magic gdb server, no needs to open a
#local sever in PC
cgdb_bmp:
	cgdb -d $(GDB) firmware.elf -x ./bmp_gdbinit.gdb
flash_openocd:
	openocd -f interface/stlink-v2.cfg \
	-f target/stm32f4x_stlink.cfg \
	-c "init" \
	-c "reset init" \
	-c "halt" \
	-c "flash write_image erase $(PROJECT).elf" \
	-c "verify_image $(PROJECT).elf" \
	-c "reset run" -c shutdown
#automatically formate
astyle: 
	astyle -r --exclude=lib  *.c *.h

.PHONY:all clean flash openocd gdbauto gdbtui cgdb astyle
