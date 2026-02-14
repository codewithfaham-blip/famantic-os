# Famantic OS - Quick Start Guide

## 🚀 Quick Start (Windows Users)

### Step 1: Install WSL

Open **PowerShell as Administrator** and run:

```powershell
wsl --install
```

**Restart your computer** after installation.

### Step 2: Install Build Tools

Open **WSL** (search for "Ubuntu" or "WSL" in Start menu) and run:

```bash
sudo apt update
sudo apt install build-essential nasm xorriso grub-pc-bin grub-common -y
```

### Step 3: Navigate to Project

In WSL terminal:

```bash
cd /mnt/c/Users/Admin/OneDrive/Desktop/Famantic-os
```

### Step 4: Build the OS

**Option A: Using the automated script**
```bash
chmod +x build.sh
./build.sh
```

**Option B: Using Make**
```bash
make
```

**Option C: Using Windows Batch File**
- Simply double-click `build-windows.bat` in Windows Explorer

### Step 5: Test in VirtualBox

1. **Open VirtualBox**
2. Click **New** to create a new VM:
   - Name: `Famantic OS`
   - Type: `Linux`
   - Version: `Other Linux (32-bit)`
   - Memory: `512 MB` (minimum)
   - Hard disk: `Do not add a virtual hard disk`

3. Click **Settings** → **Storage**:
   - Click on `Empty` under Controller: IDE
   - Click the disc icon on the right
   - Choose **Choose a disk file...**
   - Select `Famantic-os.iso`

4. Click **Settings** → **System**:
   - Uncheck `Enable EFI` (if present)
   - Boot Order: Put `Optical` first

5. Click **Start** to boot your OS!

---

## 🎮 Using Your OS

### Available Commands:

| Command | Description |
|---------|-------------|
| `help` | Show all available commands |
| `clear` | Clear the screen |
| `echo <text>` | Echo text to the screen |
| `about` | Learn about Famantic OS |
| `version` | Show OS version information |
| `reboot` | Reboot the system |
| `shutdown` | Shutdown the system |

### Example Usage:

```bash
# Clear screen
famantic@localhost:~$ clear

# Echo message
famantic@localhost:~$ echo Hello from Famantic OS!
Hello from Famantic OS!

# Show version
famantic@localhost:~$ version
Famantic OS v1.0.0 Alpha
Build date: 2026-02-14
Kernel: Famantic-x86

# Learn about the OS
famantic@localhost:~$ about
```

---

## 🔧 Troubleshooting

### Build Errors

**Error: "gcc: error: unrecognized command line option '-m32'"**
```bash
# Install 32-bit support
sudo apt install gcc-multilib g++-multilib
```

**Error: "grub-mkrescue: command not found"**
```bash
sudo apt install grub-pc-bin grub-common xorriso
```

**Error: "nasm: command not found"**
```bash
sudo apt install nasm
```

### VirtualBox Issues

**OS doesn't boot - shows "No bootable medium found"**
- Make sure ISO is properly attached to optical drive
- Check boot order in VM settings (Optical should be first)

**Black screen after boot**
- Disable EFI in System settings
- Ensure VM is set to 32-bit, not 64-bit

**Keyboard not working**
- In VirtualBox, go to Settings → System → Enable IO APIC
- Make sure you click inside the VM window to capture keyboard

---

## 📝 Next Steps

Now that you have a working OS, here are some ideas to expand it:

### Easy (Beginner):
- [ ] Add more shell commands (`ls`, `cat`, `pwd`)
- [ ] Change the color scheme
- [ ] Add ASCII art to boot screen
- [ ] Create a `date` command showing system uptime

### Medium (Intermediate):
- [ ] Implement a simple text editor
- [ ] Add support for command history (up/down arrows)
- [ ] Create a basic calculator command
- [ ] Implement tab completion

### Hard (Advanced):
- [ ] Add memory management (paging)
- [ ] Implement a simple filesystem (FAT12/FAT16)
- [ ] Add support for loading programs from disk
- [ ] Implement multitasking
- [ ] Create a graphical mode (VGA/VBE)

---

## 📚 Learning Resources

- [OSDev Wiki](https://wiki.osdev.org/) - The ultimate OS development guide
- [Writing a Simple Operating System from Scratch](https://www.cs.bham.ac.uk/~exr/lectures/opsys/10_11/lectures/os-dev.pdf)
- [The Little Book About OS Development](https://littleosbook.github.io/)
- [x86 Assembly Guide](https://www.cs.virginia.edu/~evans/cs216/guides/x86.html)
- [Intel x86 Documentation](https://www.intel.com/content/www/us/en/developer/articles/technical/intel-sdm.html)

---

**Congratulations! You've built your own operating system! 🎉**
