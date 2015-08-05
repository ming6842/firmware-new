export WORKSPACE_DIR=./program

#Primary firmware target
export FIRMWARE=./firmware

#
#include .mk
include $(WORKSPACE_DIR)/makefiles/toolchain.mk
include $(WORKSPACE_DIR)/makefiles/workspace.mk

#
#object file dir
include $(WORKSPACE_DIR)/board/vertigo-v2/board_config.mk

OBJS = $(sort $(patsubst %c, %o, $(SRCS) ))



#
#make target
all: $(FIRMWARE).bin $(FIRMWARE).elf

include $(WORKSPACE_DIR)/makefiles/rules.mk

clean:
	rm -rf $(STARTUP_OBJ)
	rm -rf $(FIRMWARE).elf
	rm -rf $(FIRMWARE).bin
	rm -f $(OBJS)
	rm -f $(DEPS)
#
#upload firmware through st-flash
flash:
	st-flash write $(FIRMWARE).bin 0x8000000

#
#create gdb server through openocd
openocd: flash
	openocd -f ./program/openocd.cfg

#
#execute cgdb 
cgdb:
	cgdb -d $(GDB) -x ./program/openocd_gdb.gdb

#
#execute gdbtui
gdbtui:
	$(GDB) -tui -x ./program/openocd_gdb.gdb

#execute gdbtui
stgdbtui:
	$(GDB) -tui -x ./program/st_util_init.gdb

#
#upload firmware through black magic probe
flash_bmp:
	$(GDB) firmware.elf -x ./program/gdb_black_magic.gdb
#


install:
	make 
	make flash_openocd

#execute and connect to black magic gdb server, no needs to open a
#local sever in PC
cgdb_bmp:
	cgdb -d $(GDB) firmware.elf -x ./program/bmp_gdbinit.gdb
flash_openocd:
	openocd -f interface/stlink-v2.cfg \
	-c "transport select hla_swd"  \
	-f target/stm32f4x_stlink.cfg \
	-c "init" \
	-c "reset init" \
	-c "halt" \
	-c "flash write_image erase $(FIRMWARE).elf" \
	-c "verify_image $(FIRMWARE).elf" \
	-c "reset run" -c shutdown
#automatically formate
astyle: 
	astyle -r --exclude=lib  *.c *.h

.PHONY:all clean flash openocd gdbauto gdbtui cgdb astyle
