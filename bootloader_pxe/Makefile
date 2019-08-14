BL_SRCS =bootloader64.S
BL_ELF =bootloader64.elf
BL_BIN =bootloader64.bin 

.PHONY:bootloader
bootloader: $(BL_BIN)

$(BL_ELF):$(BL_SRCS)
	@echo "[AS] $<"
	@gcc -m32 -I . -o $@ -c $<

$(BL_BIN):$(BL_ELF)
	@echo "[BI] $@"
	@cp $< $@
	@objcopy $@ -O binary
	@rm -rf $<