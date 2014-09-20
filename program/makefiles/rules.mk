#
#makefile rules
%.bin: %.elf
	@$(OBJCOPY) -O binary $^ $@
	@echo 'OBJCOPY $<'

%.elf: $(OBJS)
	@$(LD) $(LDFLAGS) $(ARCH_FLAGS) $(OBJS) $(LDLIBS) -o $@
	@echo 'LD $^'

DEPS = $(OBJS:.o=.d)
-include $(DEPS)

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo 'CC $<'

%.o: %.s
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo 'CC $<'

.PRECIOUS: $(OBJS) %.bin %.elf