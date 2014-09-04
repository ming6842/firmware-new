#Output files
PROJECT=firmware
EXECUTABLE=$(PROJECT).elf
BIN_IMAGE=$(PROJECT).bin
TEST_EXE=run_test
#============================================================================#
HOST_CC=gcc
#Cross Compiler
CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy
GDB=arm-none-eabi-gdb
LD=arm-none-eabi-gcc
CMSIS=./lib/CMSIS
ST=./lib/STM32F4xx_StdPeriph_Driver
EXTERNAL_DEVICE=./ext_device
MCU_PERIPH=./mcu_periph
ESTIMATOR=./estimator
ACTUATORS=./actuators
COMMON=./common
RADIO_CONTROLLER=./radio_controller
CONTROLLER =./controller
MAVLINK=./mavlink
FREERTOS=./lib/FreeRTOS
MAVLINK_LIB=./lib/mavlink
TEST=./test
#============================================================================#
BOARD_CONFIG=./vertigo_v2_config.h
DEBUG_CONFIG=./debug_config.h
CFLAGS_INCLUDE=-I./ \
	-I$(TEST) \
	-I$(ACTUATORS) \
	-I$(CONTROLLER) \
	-I$(MCU_PERIPH) \
	-I$(EXTERNAL_DEVICE) \
	-I$(ESTIMATOR)\
	-I$(COMMON)\
	-I$(RADIO_CONTROLLER) \
	-I$(FREERTOS)/Source/include \
	-I$(FREERTOS)/Source/portable/GCC/ARM_CM4F \
	-I$(CMSIS) \
	-I$(ST)/inc \
	-I$(MAVLINK) \
	-I$(MAVLINK_LIB) \
	-I$(MAVLINK_LIB)/common
CFLAGS_DEFINE= \
        -D USE_STDPERIPH_DRIVER \
        -D __FPU_PRESENT=1 \
        -D ARM_MATH_CM4 \
        -D __FPU_USED=1 \
        -include $(BOARD_CONFIG) \
        -include $(DEBUG_CONFIG) \
		-U printf -D printf=printf_base

        #__CC_ARM
CFLAGS_OPTIMIZE= \
	-O2
CFLAGS_NEW_LIB_NANO= \
	--specs=nano.specs --specs=nosys.specs  -u _printf_float
CFLAGS_WARNING= \
	-Wall \
	-Wextra \
	-Wdouble-promotion \
	-Wshadow \
	-Werror=array-bounds \
	-Wfatal-errors \
	-Wmissing-prototypes \
	-Wbad-function-cast  \
	-Wstrict-prototypes \
	-Wmissing-parameter-type

ARCH_FLAGS=-mlittle-endian -mthumb -mcpu=cortex-m4 \
	-mfpu=fpv4-sp-d16 -mfloat-abi=hard

CFLAGS=-g $(ARCH_FLAGS)\
	${CFLAGS_INCLUDE} ${CFLAGS_DEFINE} \
	${CFLAGS_WARNING}


LDFLAGS +=$(CFLAGS_NEW_LIB_NANO) --static -Wl,--gc-sections

LDFLAGS +=-T STM32F427VI_FLASH.ld
LDLIBS +=-Wl,--start-group -lm -Wl,--end-group

#============================================================================#

STARTUP=./startup_stm32f427xx.o
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
ESTIMATOR_SRC =	$(ESTIMATOR)/attitude_estimator.o \
	$(ESTIMATOR)/vertical_estimator.o \
	$(ESTIMATOR)/estimator.o 

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
				 $(CONTROLLER)/controller.o \
				 $(CONTROLLER)/simple_navigation.o

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

OBJS=	./system_stm32f4xx.o \
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
        ./interrupt.o \
        ./main.o \
        $(STARTUP) \
        $(EXTERNAL_DEVICE_SRC) $(MCU_PERIPH_SRC) \
	$(ESTIMATOR_SRC) $(ACTUATORS_SRC) $(RADIO_CONTROLLER_SRC) \
	$(COMMON_SRC) $(CONTROLLER_SRC) $(FREERTOS_SRC) \
	$(MAVLINK_SRC)
	


#Make all
all:$(BIN_IMAGE)

$(BIN_IMAGE):$(EXECUTABLE)
	@$(OBJCOPY) -O binary $^ $@
	@echo 'OBJCOPY $(BIN_IMAGE)'

$(EXECUTABLE): $(OBJS)
	@$(LD) $(LDFLAGS) $(ARCH_FLAGS) $(OBJS) $(LDLIBS) -o $@ 
	@echo 'LD $(EXECUTABLE)'

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo 'CC $<'

%.o: %.s
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo 'CC $<'

PC_SIM:$(TEST_EXE)

$(TEST_EXE):$(HOST_SRC)
	$(HOST_CC) $(HOST_CFLAG) $^ -o $@
#Make clean
clean:
	rm -rf $(STARTUP_OBJ)
	rm -rf $(EXECUTABLE)
	rm -rf $(BIN_IMAGE)
	rm -f $(OBJS)

#Make flash
flash:
	st-flash write $(BIN_IMAGE) 0x8000000

#Make openocd
openocd: flash
	openocd -f ../debug/openocd.cfg

#Make cgdb
cgdb:
	cgdb -d $(GDB) -x ./st_util_init.gdb

#Make gdbtui
gdbtui:
	$(GDB) -tui -x ./st_util_init.gdb
#Make gdbauto
gdbauto: cgdb

flash_bmp:
	$(GDB) firmware.elf -x ./gdb_black_magic.gdb
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
#============================================================================#

.PHONY:all clean flash openocd gdbauto gdbtui cgdb astyle
