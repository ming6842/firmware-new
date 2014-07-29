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
        -include $(BOARD_CONFIG) $(DEBUG_CONFIG) \
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

CFLAGS=-g3 -mlittle-endian -mthumb -mcpu=cortex-m4 \
	-mfpu=fpv4-sp-d16 -mfloat-abi=hard \
	-Wl,-T,STM32F427VI_FLASH.ld \
	${CFLAGS_INCLUDE} ${CFLAGS_DEFINE} \
	${CFLAGS_WARNING} 
	

LDFLAGS+=${CFLAGS_NEW_LIB_NANO} -lm

#============================================================================#

STARTUP=./startup_stm32f427xx.s
EXTERNAL_DEVICE_SRC = $(EXTERNAL_DEVICE)/AT24C04C.c \
			$(EXTERNAL_DEVICE)/mpu9250.c \
			$(EXTERNAL_DEVICE)/hmc5983.c \
			$(EXTERNAL_DEVICE)/lea6h_ubx.c \
			$(EXTERNAL_DEVICE)/ADS1246_MPX6115A.c

MCU_PERIPH_SRC =  \
	$(MCU_PERIPH)/i2c.c \
	$(MCU_PERIPH)/spi.c \
	$(MCU_PERIPH)/gpio.c \
	$(MCU_PERIPH)/tim.c \
	$(MCU_PERIPH)/led.c \
	$(MCU_PERIPH)/usart.c \
	$(MCU_PERIPH)/input_capture.c \
	$(MCU_PERIPH)/system_time.c
ESTIMATOR_SRC =	$(ESTIMATOR)/attitude_estimator.c \
	$(ESTIMATOR)/vertical_estimator.c \
	$(ESTIMATOR)/estimator.c 

ACTUATORS_SRC = $(ACTUATORS)/pwm.c
RADIO_CONTROLLER_SRC = $(RADIO_CONTROLLER)/radio_control.c \
	$(RADIO_CONTROLLER)/pwm_decoder.c
COMMON_SRC =$(COMMON)/test_common.c \
	$(COMMON)/memory.c \
	$(COMMON)/io.c \
	$(COMMON)/std.c

CONTROLLER_SRC = $(CONTROLLER)/attitude_stabilizer.c \
				 $(CONTROLLER)/vertical_stabilizer.c \
				 $(CONTROLLER)/navigation.c \
				 $(CONTROLLER)/flight_controller.c \
				 $(CONTROLLER)/controller.c

FREERTOS_SRC=$(FREERTOS)/Source/croutine.c \
	$(FREERTOS)/Source/list.c \
	$(FREERTOS)/Source/queue.c \
	$(FREERTOS)/Source/tasks.c \
	$(FREERTOS)/Source/timers.c \
	$(FREERTOS)/Source/portable/MemMang/heap_1.c \
	$(FREERTOS)/Source/portable/GCC/ARM_CM4F/port.c

MAVLINK_SRC=$(MAVLINK)/communication.c \
	$(MAVLINK)/mission.c \
	$(MAVLINK)/parameter.c \
	$(MAVLINK)/command_parser.c \
	$(MAVLINK)/global.c

SRC+=./system_stm32f4xx.c \
	$(CMSIS)/FastMathFunctions/arm_cos_f32.c \
	$(CMSIS)/FastMathFunctions/arm_sin_f32.c \
        $(ST)/src/misc.c \
        $(ST)/src/stm32f4xx_rcc.c \
        $(ST)/src/stm32f4xx_dma.c \
        $(ST)/src/stm32f4xx_flash.c \
        $(ST)/src/stm32f4xx_gpio.c \
        $(ST)/src/stm32f4xx_usart.c \
        $(ST)/src/stm32f4xx_tim.c \
        $(ST)/src/stm32f4xx_spi.c \
        $(ST)/src/stm32f4xx_i2c.c \
        $(ST)/src/stm32f4xx_sdio.c \
        ./interrupt.c \
        ./main.c \
        $(EXTERNAL_DEVICE_SRC) $(MCU_PERIPH_SRC) \
	$(ESTIMATOR_SRC) $(ACTUATORS_SRC) $(RADIO_CONTROLLER_SRC) \
	$(COMMON_SRC) $(CONTROLLER_SRC) $(FREERTOS_SRC) \
	$(MAVLINK_SRC)


#Make all
all:$(BIN_IMAGE)

$(BIN_IMAGE):$(EXECUTABLE)
	@$(OBJCOPY) -O binary $^ $@
	@echo '    OBJCOPY $(BIN_IMAGE)'

STARTUP_OBJ = startup_stm32f427xx.o

$(STARTUP_OBJ): $(STARTUP)
	@$(CC) $(CFLAGS) $^ -c $(STARTUP)
	@echo '    CC $(STARTUP_OBJ)'

$(EXECUTABLE):$(SRC) $(STARTUP_OBJ)
	@$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
	@echo '    CC $(EXECUTABLE)'
PC_SIM:$(TEST_EXE)

$(TEST_EXE):$(HOST_SRC)
	$(HOST_CC) $(HOST_CFLAG) $^ -o $@
#Make clean
clean:
	rm -rf $(STARTUP_OBJ)
	rm -rf $(EXECUTABLE)
	rm -rf $(BIN_IMAGE)

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
