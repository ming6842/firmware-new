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
CMSIS=../lib/CMSIS
ST=../lib/STM32F4xx_StdPeriph_Driver
EXTERNAL_DEVICE=./ext_device
MCU_PERIPH=./mcu_periph
ESTIMATOR=./estimator
#============================================================================#
CFLAGS_INCLUDE=-I$(MCU_PERIPH) \
	-I$(EXTERNAL_DEVICE) \
	-I$(ESTIMATOR)\
	-I$(CMSIS) \
	-I$(ST)/inc
CFLAGS_DEFINE= \
        -D STM32F427X \
        -D USE_STDPERIPH_DRIVER \
        -D __FPU_PRESENT=1 \
        -D ARM_MATH_CM4
        # __FPU_USED=1
        #__CC_ARM

CFLAGS=	--specs=nano.specs --specs=nosys.specs -u _printf_float -g -mlittle-endian -mthumb
CFLAGS+=-mcpu=cortex-m4
CFLAGS+=-mfpu=fpv4-sp-d16 -mfloat-abi=hard
CFLAGS+=-ffreestanding -Wall
CFLAGS+=-Wl,-T,stm32_flash.ld \
	${CFLAGS_INCLUDE} ${CFLAGS_DEFINE}

LDFLAGS+=-lm

#============================================================================#

STARTUP=$(CMSIS)/startup_stm32f4xx.s
EXTERNAL_DEVICE_SRC = $(EXTERNAL_DEVICE)/AT24C04C.c \
			$(EXTERNAL_DEVICE)/mpu9250.c
MCU_PERIPH_SRC = $(MCU_PERIPH)/test_periph.c \
	$(MCU_PERIPH)/i2c.c \
	$(MCU_PERIPH)/spi.c \
	$(MCU_PERIPH)/gpio.c \
	$(MCU_PERIPH)/tim.c \
	$(MCU_PERIPH)/usart.c 
ESTIMATOR_SRC =	$(ESTIMATOR)/attitude_estimator.c

SRC+=$(CMSIS)/system_stm32f4xx.c \
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
        $(EXTERNAL_DEVICE_SRC) $(MCU_PERIPH_SRC) $(ESTIMATOR_SRC)


#Make all
all:$(BIN_IMAGE)

$(BIN_IMAGE):$(EXECUTABLE)
	@$(OBJCOPY) -O binary $^ $@
	@echo '    OBJCOPY $(BIN_IMAGE)'

STARTUP_OBJ = startup_stm32f4xx.o

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
#automatically formate
astyle: 
	astyle -r --exclude=lib  *.c *.h
#============================================================================#

.PHONY:all clean flash openocd gdbauto gdbtui cgdb astyle
