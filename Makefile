DIRS = .
CCPARAMS = -c -g -m32 -ffreestanding -fno-asynchronous-unwind-tables -I.
ASPARAMS = -m32 -I .
LDPARAMS = -static -nostdlib --nmagic -m elf_i386

C_FILES = main.c string.c misc.c serial.c
C_OBJS = $(patsubst %.c,%.o,$(C_FILES))


ELF = pxe_boot.elf
BIN = pxe_boot.bin
BL_ELF =bootloader64.elf
BL_BIN =bootloader64.bin 
BL_PATH=./bootloader_pxe

AS_FILES = trap.S
AS_OBJS = $(patsubst %.S,%.o,$(AS_FILES))


IMAGE_DEPEND = $(C_OBJS) $(AS_OBJS)
%.o: %.c
	@echo "[CC] $<"
	@gcc $(CCPARAMS) -include def.h  -o $@ -c $<

%.o: %.S
	@echo "[AS] $<"
	@gcc $(CCPARAMS) -include def.h -o $@ -c $<

.PHONY:image
image: $(BIN)
	@echo "[Compiling] bootloader"
	@rm -rf $(BL_PATH)/$(BL_ELF) $(BL_PATH)/$(BL_BIN)

$(ELF): $(IMAGE_DEPEND)
	@echo "[LD] $@"
	@ld $(LDPARAMS) -T linker.ld -o $(ELF) $(IMAGE_DEPEND)

$(BIN):$(ELF)
	@echo "[CT] $@"
	@cp $(ELF) $(BL_PATH)/$(BIN)
	@objcopy $(BL_PATH)/$(BIN) -O binary

.PHONY:clean
clean:
	@echo "[Cleaning] kernel"
	@rm -f $(ELF) $(BL_PATH)/$(BIN) $(IMAGE_DEPEND) $(BL_PATH)/$(BL_ELF) $(BL_PATH)/$(BL_BIN)
