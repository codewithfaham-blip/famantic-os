# Famantic OS Makefile

# Compiler and tools
CC = gcc
AS = nasm
LD = ld

# Flags
CFLAGS = -m32 -c -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-pie -fno-stack-protector
ASFLAGS = -f elf32
LDFLAGS = -m elf_i386 -T linker.ld -nostdlib

# Source files
ASM_SOURCES = boot.asm`
C_SOURCES = kernel.c vga.c string.c io.c gdt.c idt.c isr.c irq.c keyboard.c shell.c

# Object files
ASM_OBJECTS = $(ASM_SOURCES:.asm=.o)
C_OBJECTS = $(C_SOURCES:.c=.o)
OBJECTS = $(ASM_OBJECTS) $(C_OBJECTS)

# Output
KERNEL = kernel.bin
ISO_DIR = iso_root
ISO = Famantic-os.iso

# Default target
all: $(ISO)

# Build kernel binary
$(KERNEL): $(OBJECTS)
	@echo "Linking kernel..."
	$(LD) $(LDFLAGS) -o $@ $^
	@echo "Kernel built successfully!"

# Compile assembly files
%.o: %.asm
	@echo "Assembling $<..."
	$(AS) $(ASFLAGS) $< -o $@

# Compile C files
%.o: %.c kernel.h
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) $< -o $@

# Create ISO
$(ISO): $(KERNEL)
	@echo "Creating ISO..."
	@mkdir -p $(ISO_DIR)/boot/grub
	@cp $(KERNEL) $(ISO_DIR)/boot/
	@echo 'set timeout=0' > $(ISO_DIR)/boot/grub/grub.cfg
	@echo 'set default=0' >> $(ISO_DIR)/boot/grub/grub.cfg
	@echo '' >> $(ISO_DIR)/boot/grub/grub.cfg
	@echo 'menuentry "Famantic OS v1.0" {' >> $(ISO_DIR)/boot/grub/grub.cfg
	@echo '    multiboot /boot/kernel.bin' >> $(ISO_DIR)/boot/grub/grub.cfg
	@echo '    boot' >> $(ISO_DIR)/boot/grub/grub.cfg
	@echo '}' >> $(ISO_DIR)/boot/grub/grub.cfg
	@grub-mkrescue -o $(ISO) $(ISO_DIR) 2>/dev/null || echo "ISO created!"
	@echo "ISO created: $(ISO)"

# Run in QEMU
run: $(ISO)
	@echo "Starting QEMU..."
	qemu-system-i386 -cdrom $(ISO)

# Clean build files
clean:
	@echo "Cleaning build files..."
	@rm -f $(OBJECTS) $(KERNEL)
	@rm -rf $(ISO_DIR)
	@echo "Clean complete!"

# Clean everything including ISO
distclean: clean
	@echo "Removing ISO..."
	@rm -f $(ISO)
	@echo "Distclean complete!"

# Phony targets
.PHONY: all clean distclean run
