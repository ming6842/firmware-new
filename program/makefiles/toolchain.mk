
HOST_CC=gcc
CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy
GDB=arm-none-eabi-gdb
LD=arm-none-eabi-gcc
AR =arm-none-eabi-ar
RANLIB =arm-none-eabi-ranlib

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

LDFLAGS =$(CFLAGS_NEW_LIB_NANO) --static -Wl,--gc-sections \
	-T $(WORKSPACE_DIR)/STM32F427VI_FLASH.ld

LDLIBS = -Wl,--start-group -lm  -L$(ESTIMATOR) -lestimator -Wl,--end-group

CFLAGS_INCLUDE=-I$(WORKSPACE_DIR)/ \
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
	-I$(SERVICES) \
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

CFLAGS=-g $(ARCH_FLAGS)\
	${CFLAGS_INCLUDE} ${CFLAGS_DEFINE} \
	${CFLAGS_WARNING} -MMD -MP