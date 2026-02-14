#!/bin/bash
# Famantic OS Build Script
# Complete automated build for the OS

set -e  # Exit on error

# Colors for output
GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
NC='\033[0m' # No Color

echo -e "${BLUE}╔══════════════════════════════════════════════════════╗${NC}"
echo -e "${BLUE}║       FAMANTIC OS - Build System v1.0               ║${NC}"
echo -e "${BLUE}╚══════════════════════════════════════════════════════╝${NC}"
echo ""

# Check for required tools
echo -e "${YELLOW}Checking build dependencies...${NC}"

command -v nasm >/dev/null 2>&1 || { echo -e "${RED}✗ NASM not found. Please install it.${NC}"; exit 1; }
echo -e "${GREEN}✓${NC} NASM found"

command -v gcc >/dev/null 2>&1 || { echo -e "${RED}✗ GCC not found. Please install it.${NC}"; exit 1; }
echo -e "${GREEN}✓${NC} GCC found"

command -v ld >/dev/null 2>&1 || { echo -e "${RED}✗ LD not found. Please install binutils.${NC}"; exit 1; }
echo -e "${GREEN}✓${NC} LD found"

command -v grub-mkrescue >/dev/null 2>&1 || { echo -e "${RED}✗ grub-mkrescue not found. Please install GRUB.${NC}"; exit 1; }
echo -e "${GREEN}✓${NC} GRUB found"

echo ""

# Clean previous build
if [ -f "Makefile" ]; then
    echo -e "${BLUE}[1/3]${NC} Cleaning previous build..."
    make clean > /dev/null 2>&1 || true
    echo -e "${GREEN}✓${NC} Clean complete!"
    echo ""
fi

# Build kernel
echo -e "${BLUE}[2/3]${NC} Building Famantic OS kernel..."
echo ""

# Assemble boot.asm
echo -e "  ${YELLOW}→${NC} Assembling boot.asm..."
nasm -f elf32 boot.asm -o boot.o
echo -e "  ${GREEN}✓${NC} boot.o created"

# Compile C files
C_FILES="kernel.c vga.c string.c io.c gdt.c idt.c isr.c irq.c keyboard.c shell.c"
for file in $C_FILES; do
    echo -e "  ${YELLOW}→${NC} Compiling $file..."
    gcc -m32 -c -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-pie -fno-stack-protector $file -o ${file%.c}.o
    echo -e "  ${GREEN}✓${NC} ${file%.c}.o created"
done

echo ""
echo -e "  ${YELLOW}→${NC} Linking kernel..."
ld -m elf_i386 -T linker.ld -nostdlib -o kernel.bin boot.o kernel.o vga.o string.o io.o gdt.o idt.o isr.o irq.o keyboard.o shell.o

if [ -f "kernel.bin" ]; then
    echo -e "  ${GREEN}✓${NC} kernel.bin created!"
else
    echo -e "  ${RED}✗${NC} Kernel build failed!"
    exit 1
fi

echo ""

# Verify Multiboot
if command -v grub-file >/dev/null 2>&1; then
    echo -e "${BLUE}[3/3]${NC} Verifying Multiboot header..."
    if grub-file --is-x86-multiboot kernel.bin; then
        echo -e "${GREEN}✓${NC} Multiboot header verified!"
    else
        echo -e "${YELLOW}⚠${NC} Warning: Multiboot verification failed (but continuing...)"
    fi
    echo ""
fi

# Create ISO
echo -e "${BLUE}[4/4]${NC} Creating bootable ISO..."
rm -rf iso_root
mkdir -p iso_root/boot/grub
cp kernel.bin iso_root/boot/

cat > iso_root/boot/grub/grub.cfg << 'EOF'
set timeout=0
set default=0

menuentry "Famantic OS v1.0" {
    multiboot /boot/kernel.bin
    boot
}
EOF

grub-mkrescue -o Famantic-os.iso iso_root 2>/dev/null
echo -e "${GREEN}✓${NC} ISO created: Famantic-os.iso"
echo ""

# Cleanup build files
echo -e "${YELLOW}Cleaning up build artifacts...${NC}"
rm -f *.o kernel.bin
rm -rf iso_root
echo -e "${GREEN}✓${NC} Cleanup complete!"
echo ""

# Success message
echo -e "${GREEN}╔══════════════════════════════════════════════════════╗${NC}"
echo -e "${GREEN}║          BUILD SUCCESSFUL! 🎉                        ║${NC}"
echo -e "${GREEN}╚══════════════════════════════════════════════════════╝${NC}"
echo ""
echo -e "${BLUE}Your OS is ready:${NC} Famantic-os.iso"
echo ""
echo -e "${YELLOW}To test in QEMU:${NC}"
echo -e "  qemu-system-i386 -cdrom Famantic-os.iso"
echo ""
echo -e "${YELLOW}To test in VirtualBox:${NC}"
echo -e "  1. Create a new VM (Linux, Other 32-bit)"
echo -e "  2. Attach Famantic-os.iso as optical drive"
echo -e "  3. Boot the VM"
echo ""
