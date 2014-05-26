#Output files
PROJECT=firmware
EXECUTABLE=$(PROJECT).elf
BIN_IMAGE=$(PROJECT).bin

#============================================================================#

#Cross Compiler
CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy
GDB=arm-none-eabi-gdb
CMSIS=./lib/CMSIS
ST=./lib/STM32F4xx_StdPeriph_Driver
FREERTOS=./lib/FreeRTOS
FATFS=./lib/fatfs
CONFIG=./config
COMMON=./common
ALGORITHM=./algorithm
DRIVER=./driver
SYSTEM=./system
SHELL_=./shell
DEBUG=./debug
ARCH=CM4F

#============================================================================#
CFLAGS_INCLUDE=-I./ \
	-I$(CONFIG) \
	-I$(CMSIS) \
	-I$(ST)/inc \
	-I$(COMMON) \
	-I$(ALGORITHM) \
	-I$(DRIVER) \
	-I$(SYSTEM) \
	-I$(SHELL_) \
	-I$(DEBUG) \
	-I$(FREERTOS)/Source/include \
	-I$(FREERTOS)/Source/portable/GCC/ARM_$(ARCH) \
	-I$(FATFS)
CFLAGS_DEFINE= \
        -D STM32F427X \
        -D USE_STDPERIPH_DRIVER \
        -D __FPU_PRESENT=1 \
        -D ARM_MATH_CM4
        # __FPU_USED=1
        #__CC_ARM

CFLAGS=-g -mlittle-endian -mthumb
CFLAGS+=-mcpu=cortex-m4
CFLAGS+=-mfpu=fpv4-sp-d16 -mfloat-abi=hard
CFLAGS+=-ffreestanding -nostdlib -Wall
CFLAGS+=-Wl,-T,stm32_flash.ld \
	${CFLAGS_INCLUDE} ${CFLAGS_DEFINE}



LDFLAGS+=-lm -lc -lgcc

#============================================================================#

STARTUP=./lib/CMSIS/startup_stm32f4xx.s
SRC+=$(CMSIS)/system_stm32f4xx.c \
	$(CMSIS)/FastMathFunctions/arm_cos_f32.c \
	$(CMSIS)/FastMathFunctions/arm_sin_f32.c

SRC+=./lib/CMSIS/system_stm32f4xx.c \
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

SRC+=$(COMMON)/stdlib.c \
	$(COMMON)/string.c

SRC+=$(ALGORITHM)/moving_average.c \
	$(ALGORITHM)/PID.c \
	$(ALGORITHM)/quaternion.c

SRC+=$(DRIVER)/delay.c \
	$(DRIVER)/flash.c \
	$(DRIVER)/i2c.c \
	$(DRIVER)/sdio.c \
	$(DRIVER)/spi.c \
	$(DRIVER)/motor.c \
	$(DRIVER)/mpu9150.c \
	$(DRIVER)/ms5611.c \
	$(DRIVER)/sensor.c \
	$(DRIVER)/serial.c \
	$(DRIVER)/led.c 

SRC+=$(SYSTEM)/AHRS.c \
	$(SYSTEM)/global.c \
	$(SYSTEM)/IMU_correct.c \
	$(SYSTEM)/interrupt.c \
	$(SYSTEM)/main.c \
	$(SYSTEM)/remote_control.c\
	$(SYSTEM)/communication.c

SRC+=$(SHELL_)/linenoise.c \
	$(SHELL_)/parser.c \
	$(SHELL_)/shell.c \
	$(SHELL_)/watch.c

SRC+=$(DEBUG)/test.c

SRC+=$(FREERTOS)/Source/croutine.c \
	$(FREERTOS)/Source/list.c \
	$(FREERTOS)/Source/queue.c \
	$(FREERTOS)/Source/tasks.c \
	$(FREERTOS)/Source/timers.c \
	$(FREERTOS)/Source/portable/MemMang/heap_4.c \
	$(FREERTOS)/Source/portable/GCC/ARM_$(ARCH)/port.c \

SRC+=$(FATFS)/diskio.c \
	$(FATFS)/ff.c \
	$(FATFS)/syscall.c \
	$(FATFS)/unicode.c \
#============================================================================#
	
#============================================================================#

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
	openocd -s /opt/openocd/share/openocd/scripts/ -f ./debug/openocd.cfg

#Make cgdb
cgdb:
	cgdb -d $(GDB) -x ./debug/openocd_gdb.gdb

#Make gdbtui
gdbtui:
	$(GDB) -tui -x ./debug/openocd_gdb.gdb

#Make gdbauto
gdbauto: cgdb
#automatically formate
astyle: 
	astyle -r --exclude=lib  *.c *.h
#============================================================================#

.PHONY:all clean flash openocd gdbauto gdbtui cgdb astyle
