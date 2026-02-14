# Codespaces Implementation & Setup Guide

Since you are using GitHub Codespaces, you have a perfect Linux environment to build and test **Famantic OS**. Follow these steps to set everything up.

## 1. Install Required Tools
In your Codespace terminal, run the following command to install the necessary build tools:

```bash
sudo apt update
sudo apt install -y build-essential nasm xorriso grub-pc-bin grub-common qemu-system-x86
```

## 2. Build the OS
You can build the OS using the provided `Makefile` or the `build.sh` script.

### Using Make (Recommended):
```bash
make
```

### Using the Build Script:
```bash
chmod +x build.sh
./build.sh
```

This will generate:
- `kernel.bin` (The compiled kernel)
- `Famantic-os.iso` (The bootable disk image)

## 3. How to Run & Test
Since Codespaces is a cloud environment, you cannot see a window (GUI) directly. However, you can verify if the build is correct.

### Check if ISO is created:
```bash
ls -lh Famantic-os.iso
```

### To run on your local machine:
1. Download `Famantic-os.iso` from the Codespace file explorer (right-click -> Download).
2. Open **VirtualBox** or **VMware** on your Windows PC.
3. Create a new "Other Linux (32-bit)" VM and use the ISO as the boot disk.

## 4. Project Structure Recap
- `boot.asm`: The entry point that switches the CPU to 32-bit mode.
- `kernel.c`: The main brain of the OS.
- `vga.c`: Handles printing text to the screen.
- `shell.c`: The interactive command-line interface.
- `keyboard.c`: Handles your typing input.

---
**Happy Coding! If you get any errors in the terminal, just paste them here.**
