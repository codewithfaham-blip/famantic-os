#!/bin/bash
# Simple build script that only requires NASM

echo "Building Simplified Famantic OS..."
echo ""

# Assemble
echo "[1/3] Assembling kernel..."
nasm -f elf32 kernel-simple.asm -o kernel-simple.o

if [ ! -f "kernel-simple.o" ]; then
    echo "ERROR: Assembly failed!"
    exit 1
fi

echo "[2/3] Linking..."
ld -m elf_i386 -T linker-simple.ld -o kernel-simple.bin kernel-simple.o

if [ ! -f "kernel-simple.bin" ]; then
    echo "ERROR: Linking failed!"
    exit 1
fi

echo "[3/3] Creating ISO..."
mkdir -p iso_root/boot/grub
cp kernel-simple.bin iso_root/boot/kernel.bin

cat > iso_root/boot/grub/grub.cfg << 'EOF'
set timeout=0
set default=0

menuentry "Famantic OS v1.0 (Simple)" {
    multiboot /boot/kernel.bin
    boot
}
EOF

grub-mkrescue -o Famantic-os-simple.iso iso_root 2>/dev/null

# Cleanup
rm -f kernel-simple.o kernel-simple.bin
rm -rf iso_root

if [ -f "Famantic-os-simple.iso" ]; then
    echo ""
    echo "✓ SUCCESS! ISO created: Famantic-os-simple.iso"
    echo ""
    echo "This is a simplified version. For the full version with shell,"
    echo "use the regular build.sh script in a Linux environment."
else
    echo "ERROR: ISO creation failed!"
    exit 1
fi
