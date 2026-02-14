# Famantic OS

**A Linux-like Operating System with Shell and Keyboard Support**

Famantic OS is a complete x86 operating system written in C and Assembly that features a working shell, keyboard input, interrupt handling, and more!

## 🎯 Features

- ✅ **Multiboot Compliant** - Works with GRUB bootloader
- ✅ **Full C Kernel** - Easy to expand and modify
- ✅ **Interactive Shell** - Command-line interface with built-in commands
- ✅ **Keyboard Driver** - PS/2 keyboard support with full input handling
- ✅ **VGA Text Driver** - Colorful text display with scrolling
- ✅ **Interrupt Handling** - IDT, ISR, and IRQ support
- ✅ **Memory Segmentation** - GDT implementation
- ✅ **Built-in Commands** - help, clear, echo, about, version, reboot, shutdown
- ✅ **Boots on Real Hardware** - Works on actual x86 computers!

## 📋 Prerequisites (Windows Users)

**Since you're on Windows, WSL is the easiest option:**

### Option 1: WSL2 (Recommended) ⭐

1. **Install WSL** (PowerShell as Administrator):
```powershell
wsl --install
```

2. **Restart your computer**

3. **Open WSL** and install build tools:
```bash
sudo apt update
sudo apt install build-essential nasm xorriso grub-pc-bin grub-common
```

### Option 2: MSYS2
Download from: https://www.msys2.org/

### Option 3: Use a Linux VM (VirtualBox/VMware)

## 🔨 Build Instructions

### For Windows Users (EASIEST):

**Method 1: Using the Batch File**
```batch
# Simply double-click: build-windows.bat
# OR run in Command Prompt:
build-windows.bat
```

**Method 2: Using WSL Bash Script**
```bash
# In WSL terminal:
cd /mnt/c/Users/Admin/OneDrive/Desktop/Famantic-os
chmod +x build.sh
./build.sh
```

**Method 3: Using Make**
```bash
# In WSL terminal:
make
```

### Manual Build (For Linux/WSL):

#### Step 1: Assemble boot.asm
```bash
nasm -f elf32 boot.asm -o boot.o
```

#### Step 2: Compile C files
```bash
gcc -m32 -c -ffreestanding -O2 -Wall -Wextra kernel.c -o kernel.o
gcc -m32 -c -ffreestanding -O2 -Wall -Wextra vga.c -o vga.o
gcc -m32 -c -ffreestanding -O2 -Wall -Wextra string.c -o string.o
gcc -m32 -c -ffreestanding -O2 -Wall -Wextra io.c -o io.o
gcc -m32 -c -ffreestanding -O2 -Wall -Wextra gdt.c -o gdt.o
gcc -m32 -c -ffreestanding -O2 -Wall -Wextra idt.c -o idt.o
gcc -m32 -c -ffreestanding -O2 -Wall -Wextra isr.c -o isr.o
gcc -m32 -c -ffreestanding -O2 -Wall -Wextra irq.c -o irq.o
gcc -m32 -c -ffreestanding -O2 -Wall -Wextra keyboard.c -o keyboard.o
gcc -m32 -c -ffreestanding -O2 -Wall -Wextra shell.c -o shell.o
```

#### Step 3: Link everything
```bash
ld -m elf_i386 -T linker.ld -nostdlib -o kernel.bin boot.o kernel.o vga.o string.o io.o gdt.o idt.o isr.o irq.o keyboard.o shell.o
```

#### Step 4: Create ISO
```bash
mkdir -p iso_root/boot/grub
cp kernel.bin iso_root/boot/
cp grub.cfg iso_root/boot/grub/
grub-mkrescue -o Famantic-os.iso iso_root
```

## 🚀 Running the OS

### In VirtualBox:
1. Create a new VM (Type: Linux, Version: Other Linux 32-bit)
2. Allocate 512MB RAM (minimum)
3. Go to Settings → Storage → Add Optical Drive
4. Select `Famantic-os.iso`
5. Start the VM

### In QEMU:
```bash
qemu-system-i386 -cdrom Famantic-os.iso
```

### In VMware:
1. Create new VM → Guest OS: Other → Other (32-bit)
2. Use ISO image for installation
3. Select `Famantic-os.iso`
4. Power on

## 📂 Project Structure

```
Famantic-os/
├── boot.asm              # Bootstrap and interrupt handlers (Assembly)
├── kernel.c              # Main kernel entry point
├── kernel.h              # Header file with all declarations
├── vga.c                 # VGA text mode driver
├── string.c              # String manipulation functions
├── io.c                  # Port I/O functions
├── gdt.c                 # Global Descriptor Table
├── idt.c                 # Interrupt Descriptor Table
├── isr.c                 # Interrupt Service Routines
├── irq.c                 # Hardware interrupt handlers
├── keyboard.c            # PS/2 keyboard driver
├── shell.c               # Interactive shell
├── linker.ld             # Linker script
├── Makefile              # Build automation
├── build.sh              # Build script (Linux/WSL)
├── build-windows.bat     # Build script (Windows)
├── grub.cfg              # GRUB configuration
├── README.md             # This file
└── Famantic-os.iso       # Bootable ISO (generated)
```

## 🎨 What You'll See

When you boot Famantic OS, you'll see:

```
================================================================================
                         FAMANTIC OS v1.0.0 Alpha
================================================================================

Welcome to Famantic OS - A Linux-like Operating System

[OK] Kernel loaded successfully
[OK] GDT initialized
[OK] IDT initialized
[OK] Interrupts enabled
[OK] Keyboard driver loaded

Type 'help' for available commands

famantic@localhost:~$ _
```

**You can type commands!** Try:
- `help` - Show all commands
- `about` - Learn about the OS
- `clear` - Clear screen
- `echo Hello World` - Echo text
- `version` - Show OS version
- `reboot` - Reboot system

## 📚 Next Steps

Want to expand your OS? Here are some ideas:

1. **Add C Language Support**: Write kernel code in C (easier than Assembly!)
2. **Keyboard Input**: Detect and display keypresses
3. **Interrupt Handling**: Set up IDT (Interrupt Descriptor Table)
4. **Memory Management**: Implement basic paging
5. **File System**: Add a simple file system
6. **Multitasking**: Run multiple processes

## 🔧 Troubleshooting

**Error: "nasm: command not found"**
- Install NASM assembler (see Prerequisites)

**Error: "ld: unrecognized option '-m elf_i386'"**
- You need 32-bit binutils support. Install `binutils` or `gcc-multilib`

**Error: "grub-mkrescue: command not found"**
- Install GRUB tools: `sudo apt install grub-pc-bin grub-common xorriso`

**ISO doesn't boot in VirtualBox**
- Make sure VM is set to 32-bit, not 64-bit
- Check that ISO is mounted in optical drive
- Enable "EFI" might need to be disabled in VM settings

## 📖 Resources

- [OSDev Wiki](https://wiki.osdev.org/) - Comprehensive OS development guide
- [Multiboot Specification](https://www.gnu.org/software/grub/manual/multiboot/multiboot.html)
- [x86 Assembly Guide](https://www.cs.virginia.edu/~evans/cs216/guides/x86.html)

## 👨‍💻 Author

Created with passion for low-level programming!

## 📄 License

Free to use, modify, and learn from!

---

**Happy OS Development! 🚀**

