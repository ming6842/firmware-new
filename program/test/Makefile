#============================================================================#
#Output files
PROJECT=firmware
EXECUTABLE=$(PROJECT).elf
BIN_IMAGE=$(PROJECT).bin
TEST_EXE=$(PROJECT)_test

#============================================================================#
#GNU Toolchain

#Host Compliler for Unit test
HOST_CC=gcc

#Cross Compiler
CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy
GDB=arm-none-eabi-gdb

#============================================================================#
#Directory

CMSIS=../lib/CMSIS
ST=../lib/STM32F4xx_StdPeriph_Driver

EXTERNAL_DEVICE=./ext_device
MCU_PERIPH=./mcu_periph
COMMON=./common
ESTIMATOR=./estimator
ACTUATORS=./actuators
RADIO_CONTROLLER=./radio_controller
CONTROLLER=./controller

#============================================================================#
#Compiler Flags

CFLAGS=-g -mlittle-endian -mthumb -mcpu=cortex-m4 \
        -mfpu=fpv4-sp-d16 -mfloat-abi=hard \
        -Wl,-T,STM32F427VI_FLASH.ld \

#Macro Definition
include ./makefiles/use_component.mk
CFLAGS+=-D STM32F427X \
	-D USE_STDPERIPH_DRIVER \
	-D __FPU_PRESENT=1 \
	-D ARM_MATH_CM4 \
	-D __FPU_USED=1 \
	$(DEFINE_COMPONENTS)

#Optimization
CFLAGS+=-O2

#New Lib Nano
CFLAGS+=--specs=nano.specs \
	--specs=nosys.specs \
	-u _printf_float

#Warning
CFLAGS+=-Wall \
	-Wextra \
	-Wdouble-promotion \
	-Wshadow \
	-Werror=array-bounds \
	-Wfatal-errors \
	-Wmissing-prototypes \
	-Wbad-function-cast \
	-Wstrict-prototypes \
	-Wmissing-parameter-type

LDFLAGS+=-lm

#============================================================================#
#Board Configuration

BOARD=./board/vertigo-v2
#BOARD=./board/qcopter-v2

CFLAGS+=-D configUSE_VERTIGO_V2_BOARD
#CFLAGS+=-D configUSE_QCOPTER_V2_BOARD

include $(BOARD)/board_config.mk

#============================================================================#
#Header And Source File

STARTUP=./startup_stm32f427xx.s

CFLAGS+=-I./
SRC+=./interrupt.c \
	./global.c \
	./main.c

CFLAGS+=-I$(CMSIS)
SRC+=$(CMSIS)/FastMathFunctions/arm_cos_f32.c \
	$(CMSIS)/FastMathFunctions/arm_sin_f32.c

CFLAGS+=-I$(ST)/inc
SRC+=./system_stm32f4xx.c \
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

CFLAGS+=-I$(BOARD)
SRC+=$(BOARD)/peripheral/led.c \
	$(BOARD)/peripheral/usart.c

CFLAGS+=-I$(EXTERNAL_DEVICE)
SRC+=$(EXTERNAL_DEVICE)/ADS1246_MPX6115A.c \
	$(EXTERNAL_DEVICE)/AT24C04C.c \
	$(EXTERNAL_DEVICE)/mpu9250.c

CFLAGS+=-I$(MCU_PERIPH)
SRC+=$(MCU_PERIPH)/i2c.c \
	$(MCU_PERIPH)/input_capture.c \
	$(MCU_PERIPH)/spi.c \
	$(MCU_PERIPH)/tim.c

CFLAGS+=-I$(COMMON)
SRC+=$(COMMON)/test_common.c

CFLAGS+=-I$(ESTIMATOR)
SRC+=$(ESTIMATOR)/attitude_estimator.c \
	$(ESTIMATOR)/vertical_estimator.c

CFLAGS+=-I$(ACTUATORS)
SRC+=$(ACTUATORS)/pwm.c

CFLAGS+=-I$(RADIO_CONTROLLER)
SRC+=$(RADIO_CONTROLLER)/pwm_decoder.c \
	$(RADIO_CONTROLLER)/radio_control.c

CFLAGS+=-I$(CONTROLLER)
SRC+=$(CONTROLLER)/attitude_stabilizer.c

#============================================================================#

#Make all
all: $(BIN_IMAGE)

$(BIN_IMAGE): $(EXECUTABLE)
	@$(OBJCOPY) -O binary $^ $@
	@echo '    OBJCOPY $(BIN_IMAGE)'

STARTUP_OBJ=startup_stm32f427xx.o

$(STARTUP_OBJ): $(STARTUP)
	@echo '    Target Board: $(TARGET_BOARD)'
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
	st-flash --reset write $(BIN_IMAGE) 0x8000000

#Make openocd
openocd: flash
	openocd -f ../debug/openocd.cfg

#Make gdbauto
gdbauto:
	cgdb -d $(GDB) -x ./st_util_init.gdb

#Make astyle
astyle: 
	astyle -r --exclude=lib  *.c *.h
#============================================================================#

.PHONY:all clean flash openocd gdbauto gdbtui cgdb astyle target
