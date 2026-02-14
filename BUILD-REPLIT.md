# Replit Build Guide (Alternative Method)

## 🚀 Build Famantic OS Using Replit (Free Online IDE)

Since WSL is not working, you can use Replit - a free online coding environment!

### Step 1: Go to Replit

1. Visit https://replit.com/
2. Sign up for FREE (use Google/GitHub)

### Step 2: Create a Bash Repl

1. Click **+ Create Repl**
2. Choose **Bash** template
3. Name it: `Famantic-OS`
4. Click **Create Repl**

### Step 3: Upload Files

In the Replit interface:
1. Click on **Files** icon (left sidebar)
2. Click the three dots **⋮** → **Upload file**
3. Upload ALL your Famantic OS files:
   - boot.asm
   - kernel.c, kernel.h
   - vga.c, string.c, io.c
   - gdt.c, idt.c, isr.c, irq.c
   - keyboard.c, shell.c
   - linker.ld
   - Makefile
   - build.sh

### Step 4: Install Build Tools

In the Replit **Console** (right side), run:

```bash
# Install required packages
apt-get update
apt-get install -y build-essential nasm xorriso grub-pc-bin grub-common mtools
```

### Step 5: Build the OS

```bash
# Make build script executable
chmod +x build.sh

# Build!
./build.sh
```

### Step 6: Download ISO

After successful build:
1. Find `Famantic-os.iso` in the Files panel
2. Right-click → **Download**

### Alternative: Use Makefile

```bash
make
```

---

## ⚠️ If Replit Doesn't Have GRUB Tools

Some Replit environments might not support GRUB. In that case, use **GitHub Codespaces** instead (see BUILD-ONLINE.md).

Or try Google Cloud Shell: https://shell.cloud.google.com/
(Completely free, no credit card needed!)

---

**Pro Tip**: Save your Repl so you can rebuild anytime!
